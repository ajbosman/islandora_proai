# Islandora PROAI DRUSH scripts

## Introduction

This directory contains two PHP scripts for updating the relationship `http://www.openarchives.org/OAI/2.0/itemID` in RELS-EXT of objects that have a PROAI activated content model and belong to a PROAI activated collection:

* update_itemids.php
* renew_itemids.php

The RELS-EXT for these objects should look like:
```
<rdf:RDF xmlns:fedora="info:fedora/fedora-system:def/relations-external#" xmlns:fedora-model="info:fedora/fedora-system:def/model#" xmlns:islandora="http://islandora.ca/ontology/relsext#" xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#">
  <rdf:Description rdf:about="info:fedora/uuid:c2c9b8de-292a-40a0-95e9-7db0edf2b491">
    <fedora:isMemberOfCollection rdf:resource="info:fedora/collection:conference"></fedora:isMemberOfCollection>
    <fedora-model:hasModel rdf:resource="info:fedora/islandora:sp_pdf"></fedora-model:hasModel>
    <fedora:isConstituentOf rdf:resource="info:fedora/uuid:46ec530c-9fb3-4aa1-983f-ccebfb32d841"></fedora:isConstituentOf>
    <islandora:isSequenceNumberOfuuid_46ec530c-9fb3-4aa1-983f-ccebfb32d841>354</islandora:isSequenceNumberOfuuid_46ec530c-9fb3-4aa1-983f-ccebfb32d841>
    <itemID xmlns="http://www.openarchives.org/OAI/2.0/">oai:tudelft.nl:uuid:c2c9b8de-292a-40a0-95e9-7db0edf2b491</itemID>
  </rdf:Description>
</rdf:RDF>
```

## Usage

### update_itemids.php

This script must be used when in the PROAI admin pages new collections are activated for PROAI or existing collections are de-activated.
And also when in the PROAI admin pages new content models are activated for PROAI or existing content models are de-activated.

Another use case is when a batch ingest has been performed.

The script first checks whether the relationship has to be removed from objects and then adds the relationship to objects that do not have one, but have a PROAI activated content model and belong to a PROAI activated collection.

```
cd /your/islandora/installation/sites/default
drush php-script ../all/modules/islandora_proai/drush/update_itemids.php
```

### renew_itemids.php.php

This script must be used when in the PROAI admin pages the OAI prefix has been changed.

The script first removes the relationship from all objects that have one and then adds the relationship to all objects that that have a PROAI activated content model and belong to a PROAI activated collection.

```
cd /your/islandora/installation/sites/default
drush php-script ../all/modules/islandora_proai/drush/renew_itemids.php
```


