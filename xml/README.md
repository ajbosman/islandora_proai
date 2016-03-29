# Islandora PROAI XML datastreams

## Introduction

The directory `/islandora_proai/xml` has xml, wsdl and xsl files, each of which contains the xml of datastreams.

Object **collection:support** has only one DC (dublin core) datastream.

* islandora_proai_collection_dc.xml

Object **aux:identify** has two datastreams: DC and IDENTIFY. 

* islandora_proai_identify_dc.xml
* islandora_proai_identify_identify.xml

Object **sdef:oai** has three datastreams: DC, METHODMAP and RELS-EXT. 

* islandora_proai_sdef_dc.xml
* islandora_proai_sdef_methodmap.xml
* islandora_proai_sdef_rels_ext.xml

Object **sdef:oai** has four datastreams: DC, METHODMAP, DSINPUTSPEC and WSDL. 

* islandora_proai_sdep_dc.xml
* islandora_proai_sdep_methodmap.xml
* islandora_proai_sdep_dsinputspec.xml
* islandora_proai_sdep_wsdl.wsdl

Object **aux:xslt** has a DC datastreams and one or more datastreams with xslt transformations. In this example: COPY, MODS2MODS and MODS2DIDL.

* islandora_proai_xslt_dc.xml
* islandora_proai_xslt_copy.xsl
* islandora_proai_xslt_mods2didl.xsl
* islandora_proai_xslt_mods2mods.xsl

## IDENTIFY

Change islandora_proai_identify_identify.xml according to your institution details:

```
<Identify>
  <repositoryName>YOUR REPOSITORY NAME</repositoryName>
  <baseURL>YOUR BASE URL</baseURL>
  <protocolVersion>2.0</protocolVersion>
  <adminEmail>EMAIL OF YOUR ADMINISTRATOR</adminEmail>
  <earliestDatestamp>2001-01-01T10:00:00Z</earliestDatestamp>
  <deletedRecord>transient</deletedRecord>
  <granularity>YYYY-MM-DDThh:mm:ssZ</granularity>
</Identify>
``` 

## Adding an XSLT datastream

### Object aux:xslt: new datastream

First a new file containing the content of an XSLT datastream must be added to the directory `/islandora_proai/xml`. It should have a name like islandora_proai_xslt_myxslt.xsl where myxslt can be replaced by the DSID of the new datastream. The ingest function of the module recognizes this name and adds a datastream MYXSLT to the **aux:xslt** object.

## Changing **sdef:oai** and **sdep:oai**

Changing the datastreams of **sdef:oai** and **sdep:oai** boils down to adding XML elements as listed below, while changing:

* [METHOD_NAME] by a short name, preferable lower case for the method, e.g. nl_didl
* [DSID_SOURCE] by the datastream id of the datastream that is to be transformed by this method, e.g. MODS 
* [DSID_XSLT]   by the datastream id of the XSLT that performs the transformation, e.g. MODS2DIDL


### Object sdef:oai: datastream METHODMAP

Add a new `fmm:Method` element
```
  <fmm:Method operationName="[METHOD_NAME]">
    <fmm:UserInputParm defaultValue="no" parmName="CLEAR_CACHE" passBy="VALUE" required="false">
      <fmm:ValidParmValues>
        <fmm:ValidParm value="yes"></fmm:ValidParm>
        <fmm:ValidParm value="no"></fmm:ValidParm>
      </fmm:ValidParmValues>
    </fmm:UserInputParm>
  </fmm:Method>
```


### Object sdep:oai: datastream METHODMAP

Add a new `fmm:Method` element. 
```
  <fmm:Method operationName="[METHOD_NAME]" wsdlMsgName="[METHOD_NAME]Request" wsdlMsgOutput="response">
    <fmm:UserInputParm defaultValue="no" parmName="CLEAR_CACHE" passBy="VALUE" required="false">
      <fmm:ValidParmValues>
        <fmm:ValidParm value="yes"></fmm:ValidParm>
        <fmm:ValidParm value="no"></fmm:ValidParm>
      </fmm:ValidParmValues>
    </fmm:UserInputParm>
    <fmm:UserInputParm defaultValue="" parmName="key" passBy="VALUE" required="false"></fmm:UserInputParm>
    <fmm:DatastreamInputParm parmName="[DSID_SOURCE]" passBy="URL_REF" required="true"/>
    <fmm:MethodReturnType wsdlMsgName="response" wsdlMsgTOMIME="N/A"/>
  </fmm:Method>
```
Note that the first line should look like: `<fmm:Method operationName="something" wsdlMsgName="somethingRequest" wsdlMsgOutput="response">`

### Object sdep:oai, datastream DSINPUTSPEC

Check whether this datastream already has an element `fbs:DSInput` with the datastream id of the datastream that is the source of the transformation. If not: add a new `fbs:DSInput` element.
```
  <fbs:DSInput DSMax="1" DSMin="1" DSOrdinality="false" wsdlMsgPartName="[DSID_SOURCE]">
    <fbs:DSInputLabel>XML source file</fbs:DSInputLabel>
    <fbs:DSMIME>text/xml</fbs:DSMIME>
    <fbs:DSInputInstruction>N/A</fbs:DSInputInstruction>
  </fbs:DSInput>
```

Add a new `fbs:DSInput` element with the datastream of the new XSLT datastream:
```
  <fbs:DSInput DSMax="1" DSMin="1" DSOrdinality="false" pid="aux:xslt" wsdlMsgPartName="[DSID_XSLT]">
    <fbs:DSInputLabel>XSLT stylesheet file</fbs:DSInputLabel>
    <fbs:DSMIME>text/xml</fbs:DSMIME>
    <fbs:DSInputInstruction>N/A</fbs:DSInputInstruction>
  </fbs:DSInput>
```


### Object sdep:oai, datastream WSDL

Add a new `wsdl:message` element:
```
  <wsdl:message name="[METHOD_NAME]Request">
            <wsdl:part name="source" type="this:URLType"></wsdl:part>
            <wsdl:part name="CLEAR_CACHE" type="this:CLEAR_CACHEType"></wsdl:part>
            <wsdl:part name="key" type="this:keyType"></wsdl:part>
  </wsdl:message>
```

Within `wsdl:portType` add a new `wsdl:operation` element:
```
    <wsdl:operation name="[METHOD_NAME]">
      <wsdl:input message="this:[METHOD_NAME]Request"/>
      <wsdl:output message="this:response"/>
    </wsdl:operation>
```

Within `wsdl:binding` add a new `wsdl:operation` element:

```
    <wsdl:operation name="[METHOD_NAME]">
      <http:operation location="SaxonServlet?source=([DSID_SOURCE])&amp;style=([DSID_XSLT])&amp;clear-stylesheet-cache=(CLEAR_CACHE)"></http:operation>
      <wsdl:input></wsdl:input>
      <wsdl:output></wsdl:output>
    </wsdl:operation>
```

## Ingest the datastreams:
Ingest the objects in the form Administration » Islandora » Islandora Utility Modules » PROAI (admin/islandora/tools/proai). Purge first if they already ingested.
