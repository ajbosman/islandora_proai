<?php

/**
 * @file
 * Proai module:
 * Separate drush script that
 *    1. removes all existing itemID relations 
 *    2. adds itemID relations to the RELS-EXT datastream of objects with a PROAI actived content model
 *       in the PROAI activated collections
 *
 * Use this script when you want to renew all itemID relations, and
 * especially whet the OAI institution ID is changed
 *
 */

define('FEDORA_URL', 'http://your.server.somewehere/fedora');
define('FEDORA_USER','fedoraAdmin');
define('FEDORA_PASSWORD','xxxxxxxx');
define('OAI_NAMESPACE','http://www.openarchives.org/OAI/2.0/');

include_once 'sites/all/libraries/tuque/RepositoryConnection.php';
include_once 'sites/all/libraries/tuque/FedoraApiSerializer.php';
include_once 'sites/all/libraries/tuque/FedoraApi.php';
include_once 'sites/all/libraries/tuque/Cache.php';
include_once 'sites/all/libraries/tuque/Repository.php';
include_once 'sites/all/libraries/tuque/Object.php';


/**
 * Updates all objects in activated collections + activated content models.
 *
 * PROAI requires an itemID relation with namespace OAI_NAMESPACE: "http://www.openarchives.org/OAI/2.0/"
 *
 * @return string
 *   A string containing the updates or an error message
 */
function islandora_proai_pids() {
  $cmodels_saved = variable_get('islandora_proai_cmodels', array());
  $collections_saved = variable_get('islandora_proai_collections',  array());
  $prefix = variable_get('islandora_proai_prefix','');
  if (strlen($prefix) > 0) $prefix .= ':';
  
  $telRemoved = 0;
  $telAdded = 0;
  
  try {
    $connection = new RepositoryConnection(FEDORA_URL, FEDORA_USER, FEDORA_PASSWORD);
    $api = new FedoraApi($connection);
    $repository = new FedoraRepository($api, new simpleCache());

    //remove
    //get from #ri subject and pid from objects that have the oai pid relation
    $query = "select distinct ?subject ?pid from <#ri> where { ".
             "?subject <".OAI_NAMESPACE."itemID> ?pid } ";
    $results = $repository->ri->sparqlQuery($query);

    if (count($results) == 0) {
      echo "No objects found with an itemID...\n\n";
    }
    else {
      //loop and remove 
      $tel = 0;
      echo "Removing...\n";
      foreach ($results as $result) {
        //
        $object = $repository->getObject($result['subject']['value']);
        $object->relationships->remove(OAI_NAMESPACE,'itemID');
        $telRemoved++;
        $tel++;
        if ($tel % 10 == 0) echo "checked: $tel, removed: $telRemoved\r";
      }
      echo "\nRemoved itemID from $telRemoved objects.\n\n";
    }
    
    //add
    if ((count($cmodels_saved) > 0) && (count($collections_saved) > 0)) {
      //loop trough collections
      foreach ($collections_saved as $coll=>$set) {
        echo "Adding in $coll ...\n";
        foreach ($cmodels_saved as $cm) {
          echo "\tAdding in $cm ...\n";
          $tel = 0;
          $query = "select distinct ?subject from <#ri> where { ".
             "?subject <info:fedora/fedora-system:def/relations-external#isMemberOfCollection> <info:fedora/".$coll."> .".
             "?subject <info:fedora/fedora-system:def/model#hasModel> <info:fedora/".$cm."> } ";
          $results = $repository->ri->sparqlQuery($query);
          
          if (count($results) == 0) {
            echo "No objects found in '".$coll."' with content model '".$cm."'...\n\n";
          }
          else {
            foreach ($results as $result) {
              $object = $repository->getObject($result['subject']['value']);
              $object->relationships->add(OAI_NAMESPACE,'itemID',$prefix.$result['subject']['value'],RELS_TYPE_PLAIN_LITERAL);
              $telAdded++;
              $tel++;
              if ($tel % 10 == 0) echo "\tchecked: $tel, added: $telAdded\r";
            }
            echo "\n\tAdded itemID to $telAdded objects.\n\n";
          }
          
        }
      }
    }
    else {
      if (count($cmodels_saved) == 0) echo "Content models not set.\n";
      if (count($collections_saved) == 0) echo "Collections not set.\n";
    }
  }
  catch (RepositoryException $e) {
    echo "Error: ".$e->getMessage()."\n";
    //dpm( 'Caught exception: '.$e->__toString());
  }
}

echo "Starting update_objects.php ".date('Y-m-d H:i:s')."\n";
islandora_proai_pids();
echo "\nFinished update_objects.php ".date('Y-m-d H:i:s')."\n\n";
?>
