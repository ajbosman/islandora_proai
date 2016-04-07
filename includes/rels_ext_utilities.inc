<?php

/**
* @file
* Proai module:
* Houses the utility functions that manage rels_ext or retrieve triples.
*/

define('FEDORA_URL', 'http://your.server.somewhere/fedora');
define('FEDORA_USER','xxxxxxxxx');
define('FEDORA_PASSWORD','xxxxxxxxx');
define('OAI_NAMESPACE','http://www.openarchives.org/OAI/2.0/');

include_once 'sites/all/libraries/tuque/RepositoryConnection.php';
include_once 'sites/all/libraries/tuque/FedoraApiSerializer.php';
include_once 'sites/all/libraries/tuque/FedoraApi.php';
include_once 'sites/all/libraries/tuque/Cache.php';
include_once 'sites/all/libraries/tuque/Repository.php';
include_once 'sites/all/libraries/tuque/Object.php';

/**
* Purge required objects.
*
* @return message
*   A string in which purged objects are listed or errors when purging did not succeed
*/
function islandora_proai_purge_objects($obj_ids = NULL) {
  if (is_null($obj_ids)) {
    $obj_ids = array('collection:support','aux:oai_identify','sdef:oai','sdep:oai','aux:xslt');
  }
  else if (is_string($obj_ids)) {
    $obj_ids = array($obj_ids);
  }
  else {
    $obj_ids = array();
  }
  $message = "";

  try {
    $connection = new RepositoryConnection(FEDORA_URL, FEDORA_USER, FEDORA_PASSWORD);
    $api = new FedoraApi($connection);
    $repository = new FedoraRepository($api, new simpleCache());

    if (in_array('collection:support',$obj_ids)) {
      try {
        $object = $repository->getObject('collection:support');
        $repository->purgeObject($object);
        $message .= " 'collection:support' purged.";
      }
      catch (RepositoryException $e) {
        $message .= " 'collection:support' error: ".$e->getMessage().".";
        //dpm( 'Caught exception: '.$e->__toString());
      }
    }

    if (in_array('aux:oai_identify',$obj_ids)) {
      try {
        $object = $repository->getObject('aux:oai_identify');
        $repository->purgeObject($object);
        $message .= " 'aux:oai_identify' purged.";
      }
      catch (RepositoryException $e) {
        $message .= " 'aux:oai_identify' error: ".$e->getMessage().".";
        //dpm( 'Caught exception: '.$e->__toString());
      }
    }

    if (in_array('sdef:oai',$obj_ids)) {
      try {
        $object = $repository->getObject('sdef:oai');
        $repository->purgeObject($object);
        $message .= " 'sdef:oai' purged.";
      }
      catch (RepositoryException $e) {
        $message .= " 'sdef:oai' error: ".$e->getMessage().".";
        //dpm( 'Caught exception: '.$e->__toString());
      }
    }

    if (in_array('sdep:oai',$obj_ids)) {
      try {
        $object = $repository->getObject('sdep:oai');
        $repository->purgeObject($object);
        $message .= " 'sdep:oai' purged.";
      }
      catch (RepositoryException $e) {
        $message .= " 'sdep:oai' error: ".$e->getMessage().".";
        //dpm( 'Caught exception: '.$e->__toString());
      }
    }

    if (in_array('aux:xslt',$obj_ids)) {
      try {
        $object = $repository->getObject('aux:xslt');
        $repository->purgeObject($object);
        $message .= " 'aux:xslt' purged.";
      }
      catch (RepositoryException $e) {
        $message .= " 'aux:xslt' error: ".$e->getMessage().".";
        //dpm( 'Caught exception: '.$e->__toString());
      }
    }
  }
  catch (RepositoryException $e) {
    $message = "Error: ".$e->getMessage();
    //dpm( 'Caught exception: '.$e->__toString());
  }
  return $message;
}

/**
* Ingests required objects.
*
* @return message
*   A string in which ingested objects are listed or errors when ingesting did not succeed
*/
function islandora_proai_ingest_objects($obj_ids = NULL) {
  if (is_null($obj_ids)) {
    $obj_ids = array('collection:support','aux:oai_identify','sdef:oai','sdep:oai','aux:xslt');
  }
  else if (is_string($obj_ids)) {
    $obj_ids = array($obj_ids);
  }
  else {
    $obj_ids = array();
  }
  $module_path = drupal_get_path('module', 'islandora_proai');
  $message = "Ingested objects:";
  try {
    $connection = new RepositoryConnection(FEDORA_URL, FEDORA_USER, FEDORA_PASSWORD);
    $api = new FedoraApi($connection);
    $repository = new FedoraRepository($api, new simpleCache());

    //collection:support
    if (in_array('collection:support',$obj_ids)) {
      try {
        $coll_object = $repository->constructObject('collection:support');
        $coll_object->owner = 'fedoraAdmin';
        $coll_object->label = 'Support - objects supporting Fedora objects, e.g. disseminations';
        //$coll_object->relationships->remove();
        $coll_object->relationships->add(FEDORA_MODEL_URI, 'hasModel', 'islandora:collectionCModel');
        // DC Datastream.
        $ds = $coll_object->constructDatastream('DC', 'X');
        $ds->label = 'Dublin Core Record for this object';
        $ds->mimetype = 'text/xml';
        $ds->setContentFromFile("$module_path/xml/islandora_proai_collection_dc.xml", FALSE);
        $coll_object->ingestDatastream($ds);
        $repository->ingestObject($coll_object);
        $message .= " 'collection:support'";
      }
      catch (RepositoryException $e) {
        $message .= " 'collection:support' error: ".$e->getMessage().".";
        //dpm( 'Caught exception: '.$e->__toString());
      }
    }
    
    //aux:oai_identify
    if (in_array('aux:oai_identify',$obj_ids)) {
      try {
        $identify_object = $repository->constructObject('aux:oai_identify');
        $identify_object->owner = 'fedoraAdmin';
        $identify_object->label = 'XML response to OAI Identify requests';
        //$identify_object->relationships->remove();
        $identify_object->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOfCollection', 'collection:support');
        // DC Datastream.
        $ds = $identify_object->constructDatastream('DC', 'X');
        $ds->label = 'Dublin Core Record for this object';
        $ds->mimetype = 'text/xml';
        $ds->setContentFromFile("$module_path/xml/islandora_proai_identify_dc.xml", FALSE);
        $identify_object->ingestDatastream($ds);
        // IDENTIFY datastream.
        $ds = $identify_object->constructDatastream('IDENTIFY', 'X');
        $ds->label = 'OAI Identify response xml';
        $ds->mimetype = 'text/xml';
        $ds->setContentFromFile("$module_path/xml/islandora_proai_identify_identify.xml", FALSE);
        $identify_object->ingestDatastream($ds);
        $repository->ingestObject($identify_object);
        $message .= " 'aux:oai_identify'";
      }
      catch (RepositoryException $e) {
        $message .= " 'aux:oai_identify' error: ".$e->getMessage().".";
        //dpm( 'Caught exception: '.$e->__toString());
      }
    }

    //sdef:oai
    if (in_array('sdef:oai',$obj_ids)) {
      try {
        $sdef_object = $repository->constructObject('sdef:oai');
        $sdef_object->owner = 'fedoraAdmin';
        $sdef_object->label = 'Service Definition for oai set description';
        //$sdef_object->relationships->remove();
        $sdef_object->relationships->add(FEDORA_MODEL_URI, 'hasModel', 'fedora-system:ServiceDefinition-3.0');
        $sdef_object->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOfCollection', 'collection:support');
        // DC Datastream.
        $ds = $sdef_object->constructDatastream('DC', 'X');
        $ds->label = 'Dublin Core Record for this object';
        $ds->mimetype = 'text/xml';
        $ds->setContentFromFile("$module_path/xml/islandora_proai_sdef_dc.xml", FALSE);
        $sdef_object->ingestDatastream($ds);
        // METHODMAP Datastream.
        $ds = $sdef_object->constructDatastream('METHODMAP', 'X');
        $ds->label = 'Service Definition Method Map';
        $ds->mimetype = 'text/xml';
        $ds->setContentFromFile("$module_path/xml/islandora_proai_sdef_methodmap.xml", FALSE);
        $sdef_object->ingestDatastream($ds);
        $repository->ingestObject($sdef_object);
        $message .= " 'sdef:oai'";
      }
      catch (RepositoryException $e) {
        $message .= " 'sdef:oai' error: ".$e->getMessage().".";
        //dpm( 'Caught exception: '.$e->__toString());
      }
    }

    //sdep:oai
    if (in_array('sdep:oai',$obj_ids)) {
      try {
        $sdep_object = $repository->constructObject('sdep:oai');
        $sdep_object->owner = 'fedoraAdmin';
        $sdep_object->label = 'Service Deployment for oai set description';
        //$sdep_object->relationships->remove();
        $sdep_object->relationships->add(FEDORA_MODEL_URI, 'hasModel', 'fedora-system:ServiceDeployment-3.0');
        $sdep_object->relationships->add(FEDORA_MODEL_URI, 'isDeploymentOf', 'sdef:oai');
        //isContractorOf is managed by content models tab of the admin form of islandora proai
        $sdep_object->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOfCollection', 'collection:support');
        // DC Datastream.
        $ds = $sdep_object->constructDatastream('DC', 'X');
        $ds->label = 'Dublin Core Record for this object';
        $ds->mimetype = 'text/xml';
        $ds->setContentFromFile("$module_path/xml/islandora_proai_sdep_dc.xml", FALSE);
        $sdep_object->ingestDatastream($ds);
        // METHODMAP Datastream.
        $ds = $sdep_object->constructDatastream('METHODMAP', 'X');
        $ds->label = 'Deployment Method Map';
        $ds->mimetype = 'text/xml';
        $ds->setContentFromFile("$module_path/xml/islandora_proai_sdep_methodmap.xml", FALSE);
        $sdep_object->ingestDatastream($ds);
        // DSINPUTSPEC Datastream
        $ds = $sdep_object->constructDatastream('DSINPUTSPEC', 'X');
        $ds->label = 'Datastream Input Specification';
        $ds->mimetype = 'text/xml';
        $ds->setContentFromFile("$module_path/xml/islandora_proai_sdep_dsinputspec.xml", FALSE);
        $sdep_object->ingestDatastream($ds);
        // WSDL Datastream
        $ds = $sdep_object->constructDatastream('WSDL', 'X');
        $ds->label = 'WSDL Bindings';
        $ds->mimetype = 'text/xml';
        $ds->setContentFromFile("$module_path/xml/islandora_proai_sdep_wsdl.wsdl", FALSE);
        $sdep_object->ingestDatastream($ds);
        $repository->ingestObject($sdep_object);
        $message .= " 'sdep:oai'";
      }
      catch (RepositoryException $e) {
        $message .= " 'sdep:oai' error: ".$e->getMessage().".";
        //dpm( 'Caught exception: '.$e->__toString());
      }
    }
    
    //aux:xslt
    if (in_array('aux:xslt',$obj_ids)) {
      try {
        $xslt_object = $repository->constructObject('aux:xslt');
        $xslt_object->owner = 'fedoraAdmin';
        $xslt_object->label = 'XSLTs for disseminations';
        //$xslt_object->relationships->remove();
        $xslt_object->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOfCollection', 'collection:support');
        // DC Datastream.
        $ds = $xslt_object->constructDatastream('DC', 'X');
        $ds->label = 'Dublin Core Record for this object';
        $ds->mimetype = 'text/xml';
        $ds->setContentFromFile("$module_path/xml/islandora_proai_xslt_dc.xml", FALSE);
        $xslt_object->ingestDatastream($ds);

        // settings Datastream.
        $ds = $xslt_object->constructDatastream('SETTINGS', 'X');
        $ds->label = 'XSLT settings';
        $ds->mimetype = 'text/xml';
        
        //replace <!--PREFIXPARAM--> by <xsl:param name="oaiPrefix" select="&apos;ISLANDORA_PROAI_PREFIX&apos;"></xsl:param> 
        $prefix = variable_get('islandora_proai_prefix','');
        $settings = file_get_contents("$module_path/xml/islandora_proai_xslt_settings.xsl");
        //
        if (strlen($prefix) > 0) {
          $settings = str_replace('<!--PREFIXPARAM-->','<xsl:param name="oaiPrefix" select="'."'".$prefix."'".'"></xsl:param>',$settings);
        }
        $ds->setContentFromString($settings);
        $xslt_object->ingestDatastream($ds);

        //TODO ingest other datastreams
        $files = scandir("$module_path/xml/");
        foreach ($files as $f) {
          $parts = explode('.',$f);
          if (count($parts) > 0) {
            $first = array_shift($parts);
            if (strpos($first,'islandora_proai_xslt_') === 0) {
              $dsid = strtolower(str_replace('islandora_proai_xslt_','',$first));
              if (!in_array($dsid,array('dc','settings','rels-ext','policy'))) {
                $dsid = strtoupper($dsid);
                $ds = $xslt_object->constructDatastream($dsid, 'X');
                $ds->label = "Extra XSLT: $dsid";
                $ds->mimetype = 'text/xml';
                $ds->setContentFromFile("$module_path/xml/$f", FALSE);
                $xslt_object->ingestDatastream($ds);
              }
            }
          }
        }
        $repository->ingestObject($xslt_object);
        $message .= " 'aux:xslt'";
      }
      catch (RepositoryException $e) {
        $message .= " 'aux:xslt' error: ".$e->getMessage().".";
        //dpm( 'Caught exception: '.$e->__toString());
      }
    }
  }
  catch (Exception $e) {
    $message = "Error: ".$e->getMessage();
  }
  return $message;
}


/**
* Gets configuration data shown on the admin screen.
*
* @return array
*   An array that contains all collected data: ingested objects, activated collections and conten models and
*   information on required RELS-EXT relations.
*/
function islandora_proai_get_configuration() {
  $collections_saved = variable_get('islandora_proai_collections',  array());
  $cmodels_saved = variable_get('islandora_proai_cmodels', array());

  $configuration = array();
  $configuration['required'] = array();
  $configuration['cmodels'] = array();
  $configuration['collections'] = array();

  try {
    $connection = new RepositoryConnection(FEDORA_URL, FEDORA_USER, FEDORA_PASSWORD);
    $api = new FedoraApi($connection);
    $repository = new FedoraRepository($api, new simpleCache());

    //check required objects
    //collection:support
    $configuration['required']['collection:support'] = "Ok.";
    try {
      $coll_object = $repository->getObject('collection:support');
    }
    catch (RepositoryException $e) {
      $configuration['required']['collection:support'] = "Object 'collection:support' not found, purge and ingest PROAI required objects.";
      //dpm( 'Caught exception: '.$e->__toString());
    }

    //aux:oai_identify
    $configuration['required']['aux:oai_identify'] = 'Ok';
    try {
      $identify_object = $repository->getObject('aux:oai_identify');
      $datastream = $identify_object['IDENTIFY'];
      $configuration['required']['aux:oai_identify'] .= '.<br/><pre>'.htmlspecialchars($datastream->content).'</pre>';
    }
    catch (RepositoryException $e) {
      $configuration['required']['aux:oai_identify'] =  "Object 'aux:oai_identify' not found, purge and ingest PROAI required objects.";
      //dpm( 'Caught exception: '.$e->__toString());
    }

    //sdef:oai
    $configuration['required']['sdef:oai'] = "Ok.";
    try {
      $sdef_object = $repository->getObject('sdef:oai');
    }
    catch (RepositoryException $e) {
      $configuration['required']['sdef:oai'] = "Object 'sdef:oai' not found, purge and ingest PROAI required objects.";
      //dpm( 'Caught exception: '.$e->__toString());
    }

    //sdep:oai
    $configuration['required']['sdep:oai'] = "Ok.";
    try {
      $sdep_object = $repository->getObject('sdep:oai');
      //check isContractorOf relations of sdep object
      if (count($cmodels_saved) > 0) {
        $contractors = $sdep_object->relationships->get(FEDORA_MODEL_URI,'isContractorOf');
        $cm_from_sdep = array();
        foreach ($contractors as $contract) {
          $cm_from_sdep[] = $contract['object']['value'];
        }
        if ((count(array_diff($cm_from_sdep, $cmodels_saved)) > 0) ||
        (count(array_diff($cmodels_saved, $cm_from_sdep)) > 0)) {
          $configuration['required']['sdep:oai'] .= "<br/>'sdep:oai' does not have required RELS-EXT settings. Resubmit PROAI Content Models.";
        }
      }
    }
    catch (RepositoryException $e) {
      $configuration['required']['sdep:oai'] = "Object 'sdep:oai' not found, purge and ingest PROAI required objects.";
      //dpm( 'Caught exception: '.$e->__toString());
    }

    //aux:xslt
    $configuration['required']['aux:xslt'] = "Ok.";
    try {
      $xslt_object = $repository->getObject('aux:xslt');
      $xslts = array();
      foreach ($xslt_object as $datastream) {
        if (!in_array($datastream->id, array('DC','RELS-EXT'))) {
          $xslts[] = $datastream->id;
        }
      }
      if (count($xslts) > 0) $configuration['required']['aux:xslt'] .= '<br/>Imported XSLT datastreams: '.implode(', ',$xslts);
    }
    catch (RepositoryException $e) {
      $configuration['required']['aux:xslt'] = "Object 'aux:xslt' not found, purge and ingest PROAI required objects.";
      //dpm( 'Caught exception: '.$e->__toString());
    }

    //check content models
    if (count($cmodels_saved) > 0) {
      foreach ($cmodels_saved as $cm) {
        try {
          $cm_object = $repository->getObject($cm);
          $hasService = $cm_object->relationships->get(FEDORA_MODEL_URI,'hasService');
          if ((count($hasService) == 1) &&
          ($hasService[0]['object']['value'] == 'sdef:oai')) {
            $configuration['cmodels'][$cm] = 'Ok.';
          }
          else {
            $configuration['cmodels'][$cm] = "Object 'sdep:oai' does not have required RELS-EXT settings. Resubmit PROAI Content Models.";
          }
        }
        catch (RepositoryException $e) {
          $configuration['cmodels'][$cm] = "Object '".$cm."' not found. Resubmit PROAI Content Models.";
          //dpm( 'Caught exception: '.$e->__toString());
        }
      }
    }
    else {
      $configuration['cmodels']['-'] = 'No content models enabled. See PROAI Content Models.';
    }

    //check collections
    if (count($collections_saved) > 0) {
      foreach ($collections_saved as $coll=>$set) {
        try {
          $coll_object = $repository->getObject($coll);
          $setSpec = $coll_object->relationships->get(OAI_NAMESPACE,'setSpec');
          $setName = $coll_object->relationships->get(OAI_NAMESPACE,'setName');
          if ((count($setSpec) == 1) &&
          (count($setName) == 1) &&
          ($setSpec[0]['object']['value'] === $set) &&
          ($setName[0]['object']['value'] === $set)) {
            $configuration['collections'][$coll] = $set;
          }
          else {
            $configuration['collections'][$coll] = "Collection does not have required RELS-EXT settings. Resubmit PROAI Collections.";
          }
        }
        catch (RepositoryException $e) {
          $configuration['collections'][$coll] = "Object '".$coll."' not found. Resubmit PROAI Collections.";
          //dpm( 'Caught exception: '.$e->__toString());
        }
      }
    }
    else {
      $configuration['collections']['-'] = 'No collections enabled. See PROAI Collections.';
    }
  }
  catch (RepositoryException $e) {
    //dpm( 'Caught exception: '.$e->__toString());
  }

  return $configuration;
}

/**
* Updates RELS-EXT of activated collections.
*
* PROAI requires a setSpec and a setName relation with namespace OAI_NAMESPACE: "http://www.openarchives.org/OAI/2.0/"
*
* @return string
*   A string containing the updates or an error message
*/
function islandora_proai_update_collections($collections_old,$collections_new) {
  $message = '';
  try {
    $connection = new RepositoryConnection(FEDORA_URL, FEDORA_USER, FEDORA_PASSWORD);
    $api = new FedoraApi($connection);
    $repository = new FedoraRepository($api, new simpleCache());
    //remove relations
    foreach ($collections_old as $coll=>$set) {
      //todo: remove only when not in $collections_new
      $coll_object = $repository->getObject($coll);
      $coll_object->relationships->remove(OAI_NAMESPACE,'setSpec');
      $coll_object->relationships->remove(OAI_NAMESPACE,'setName');
      $message .= " removed from: '$coll'";
    }
    //add
    foreach ($collections_new as $coll=>$set) {
      //todo: add only when not present
      $coll_object = $repository->getObject($coll);
      $coll_object->relationships->add(OAI_NAMESPACE,'setSpec',$set,RELS_TYPE_PLAIN_LITERAL);
      $coll_object->relationships->add(OAI_NAMESPACE,'setName',$set,RELS_TYPE_PLAIN_LITERAL);
      $message .= " added to: '$coll'";
    }

  }
  catch (RepositoryException $e) {
    $message = "Error: ".$e->getMessage();
    //dpm( 'Caught exception: '.$e->__toString());
  }
  return $message;
}

/**
* Updates RELS-EXT of activated content models.
*
* PROAI requires a hasService and isContractor of relations
*
* @return string
*   A string containing the updates or an error message
*/
function islandora_proai_update_content_models($cmodels_saved, $cmodels_new) {
  $message = '';
  try {
    $connection = new RepositoryConnection(FEDORA_URL, FEDORA_USER, FEDORA_PASSWORD);
    $api = new FedoraApi($connection);
    $repository = new FedoraRepository($api, new simpleCache());

    //remove relations hasService from content models
    foreach ($cmodels_saved as $cm) {
      //todo: remove only when not in $cmodels_new
      $cm_object = $repository->getObject($cm);
      $cm_object->relationships->remove(FEDORA_MODEL_URI,'hasService');
      $message .= " removed from: '$cm'";
    }
    //add relations hasService to content models
    foreach ($cmodels_new as $cm) {
      //todo: add only when not present
      $cm_object = $repository->getObject($cm);
      $cm_object->relationships->add(FEDORA_MODEL_URI,'hasService','sdef:oai');
      $message .= " added to: '$cm'";
    }

    //add relations of sdep to content models
    $sdep = $repository->getObject('sdep:oai');
    //remove all isContractorOf relations
    $sdep->relationships->remove(FEDORA_MODEL_URI,'isContractorOf');
    //add
    foreach ($cmodels_new as $cm) {
      $rt = $sdep->relationships->add(FEDORA_MODEL_URI, 'isContractorOf', $cm);
    }
    $message .= " updated: 'sdep:oai'";
  }
  catch (RepositoryException $e) {
    $message = "Error: ".$e->getMessage();
    //dpm( 'Caught exception: '.$e->__toString());
  }
  return $message;
}


/**
* Get subcollections recursively.
*
* If started with islandora:root or collection:root all subcollections, subsubcollections etc. are returned
*
* @param $collection
*   The collection to start with.
* @param array &list
*   An array representing the list of subcollections.
*
* @return array
*   An array with pid's of the collections as keys and a string as values.
*/
function islandora_proai_get_collections($collection,&$list,$prefix) {
  $connection = new RepositoryConnection(FEDORA_URL, FEDORA_USER, FEDORA_PASSWORD);
  $api = new FedoraApi($connection);
  $repository = new FedoraRepository($api, new simpleCache());

  $query = "SELECT DISTINCT ?subject ?label FROM <#ri> WHERE {".
  "?subject <info:fedora/fedora-system:def/relations-external#isMemberOfCollection> <info:fedora/".$collection.">. ".
  "?subject <info:fedora/fedora-system:def/model#hasModel> <info:fedora/islandora:collectionCModel>. ".
  "?subject <info:fedora/fedora-system:def/model#label> ?label. } order by ?label";
  $results = $repository->ri->sparqlQuery($query);

  foreach ($results as $result) {
    $collection = $result['subject']['value'];
    $label = $result['label']['value'];
    $prefix = '-'.$prefix;
    $list[$collection] = $prefix.$label.' ('.$collection.')';
    //recurse:
    islandora_proai_get_collections($result['subject']['value'],$list,$prefix);
    $prefix = substr($prefix,1);
  }
  return $list;
}


/**
* Updates all objects in activated collections + activated content models.
*
* PROAI requires an itemID relation with namespace OAI_NAMESPACE: "http://www.openarchives.org/OAI/2.0/"
*
* @return string
*   A string containing the updates or an error message
*/
function islandora_proai_pids() {
  $message = '';
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

    //add
    if ((count($cmodels_saved) > 0) && (count($collections_saved) > 0)) {
      //loop trough collections
      foreach ($collections_saved as $coll=>$set) {
        foreach ($cmodels_saved as $cm) {

          //note 'limit 50' at the end of the query, use the Drush scripts to update all objects
          $query = "select distinct ?subject from <#ri> where { ".
          "?subject <info:fedora/fedora-system:def/relations-external#isMemberOfCollection> <info:fedora/".$coll."> .".
          "?subject <info:fedora/fedora-system:def/model#hasModel> <info:fedora/".$cm."> } limit 50";
          $results = $repository->ri->sparqlQuery($query);
          foreach ($results as $result) {
            $object = $repository->getObject($result['subject']['value']);
            if (count($object->relationships->get(OAI_NAMESPACE,'itemID')) == 0) {
              $object->relationships->add(OAI_NAMESPACE,'itemID',$prefix.$result['subject']['value'],RELS_TYPE_PLAIN_LITERAL);
              $telAdded++;
            }
          }
        }
      }
    }
    $message ="removed from $telRemoved objects, added to $telAdded objects.";
  }
  catch (RepositoryException $e) {
    $message = "Error: ".$e->getMessage();
    //dpm( 'Caught exception: '.$e->__toString());
  }
  return $message;
}

