<xsl:stylesheet xmlns:decoder="java:java.net.URLDecoder" xmlns:ds="http://www.library.tudelft.nl/ns/dataset/" xmlns:file="java.io.File" xmlns:didl="urn:mpeg:mpeg21:2002:02-DIDL-NS"
  xmlns:sparql="http://www.w3.org/2001/sw/DataAccess/rf1/result" xmlns:fr="info:fedora/fedora-system:def/relations-external#" xmlns:geo="http://www.w3.org/2003/01/geo/wgs84_pos#"
  xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" xmlns:mods="http://www.loc.gov/mods/v3" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  version="2.0">

  <!-- thisPid is a parameter defined in sdep:oai, containing the actual Fedora PID. 
       st is a parameter defining whether the stylesheet should use nl_didl rules
       See METHODMAP and WSDL. 
       Background remarks on pid in: https://wiki.duraspace.org/display/FEDORA34/Creating+a+Service+Deployment
       thisPid and st must be declared without a value otherwise they are ignored 
  <xsl:variable name="thisPid" />
  <xsl:variable name="st" /> -->
  <xsl:variable name="st" select="'didl'"/>
  <xsl:variable name="thisPid" select="'uuid:79ed6374-697f-47cb-84c4-3f4b18c3c53b'"/>
  <xsl:variable name="sparqlPid" select="concat('info:fedora/',$thisPid)"/>

  <!-- Do not change the following line: it is replaced by the islandora_proai module with an admin parameter -->
  <!--PREFIXPARAM-->
  <xsl:variable name="oaiPrefix" select="'oai:tudelft.nl'"/>

  <!-- on the server "local" must be 'http://localhost:8080/'
  <xsl:variable name="local" select="'http://localhost:8080/'"/> -->
  <xsl:variable name="local" select="'http://repo3dev.tudelft.nl/'"/>

  <xsl:variable name="urnNBNTudelft" select="'urn:NBN:nl:ui:24-'"/>
  <xsl:variable name="resolver" select="'http://resolver.tudelft.nl/'"/>
  <xsl:variable name="genrePrefix" select="'info:eu-repo/semantics/'"/>
  <xsl:param name="debug" select="false()"/>

  <xsl:variable name="error">
    <ds:error/>
  </xsl:variable>

  <xsl:param name="sparql-pars" select="'type=tuples&amp;lang=sparql&amp;format=Sparql&amp;distinct=on&amp;query='"/>
  <xsl:function name="ds:riResultAll">
    <!-- returns an xml document or an error -->
    <xsl:param name="query"/>
    <xsl:variable name="query-uri" select="concat($local,'fedora/risearch?',$sparql-pars,encode-for-uri(normalize-space($query)))"/>
    <xsl:sequence select="if (doc-available($query-uri)) then doc($query-uri) else $error"/>
    <xsl:if test="$debug">
      <xsl:message select="'ds:riResultAll',$query"/>
    </xsl:if>
  </xsl:function>

  <xsl:function name="ds:riQuery">
    <xsl:param name="pid"/>
    <xsl:param name="rel"/>
    <xsl:sequence select="concat('select ?o where { &lt;', $pid,'&gt; &lt;',$rel,'&gt; ?o }')"/>
  </xsl:function>

  <xsl:variable name="dateModifiedQuery" select="ds:riQuery($sparqlPid,'info:fedora/fedora-system:def/view#lastModifiedDate')"/>
  <xsl:variable name="dateModified" select="ds:riResultAll($dateModifiedQuery)//sparql:o/text()"/>

  <xsl:param name="download1" select="'http://repo3dev.tudelft.nl/islandora/object/'"/>
  <xsl:param name="download2" select="'/datastream/'"/>
  <xsl:param name="download3" select="'/download'"/>
  <xsl:variable name="dsQuery" select="ds:riQuery($sparqlPid,'info:fedora/fedora-system:def/view#disseminates')"/>
  <xsl:variable name="dsList" select="ds:riResultAll($dsQuery)//sparql:o[contains(@uri,'/OBJ')]"/>

</xsl:stylesheet>
