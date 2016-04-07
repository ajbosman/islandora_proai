<xsl:stylesheet
  xmlns:decoder="java:java.net.URLDecoder"
  xmlns:ds="http://www.library.tudelft.nl/ns/dataset/"
  xmlns:file="java.io.File"
  xmlns:fr="info:fedora/fedora-system:def/relations-external#"
  xmlns:sparql="http://www.w3.org/2001/sw/DataAccess/rf1/result"
  xmlns:geo="http://www.w3.org/2003/01/geo/wgs84_pos#"
  xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
  xmlns:mods="http://www.loc.gov/mods/v3"
  xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="2.0"
  >
  <!-- thisPid is a parameter defined in sdep:oai. See METHODMAP and WSDL. 
       Background remarks on pid in: https://wiki.duraspace.org/display/FEDORA34/Creating+a+Service+Deployment
       thisPid must be declared otherwise it is ignored -->
  <xsl:param name="thisPid" />
  
  <!-- Do not change the following line: it is replaced by the islandora_proai module with an admin parameter -->
  <!--PREFIXPARAM-->

  <xsl:param name="urnNBNTudelft" select="'urn:NBN:nl:ui:24-'"/>
  <xsl:param name="local" select="'http://repo3dev.tudelft.nl/'"/>
  <xsl:param name="debug" select="false()"/>
  <xsl:param name="sparql-pars"
    select="'type=tuples&amp;lang=sparql&amp;format=Sparql&amp;distinct=on&amp;query='"/>

  <xsl:variable name="error">
    <ds:error/>
  </xsl:variable>

  <xsl:function name="ds:riResultAll">
    <!-- returns an xml document or an error -->
    <xsl:param name="query"/>
    <xsl:variable name="query-uri"
      select="concat($local,'fedora/risearch?',$sparql-pars,encode-for-uri(normalize-space($query)))"/>
    <xsl:sequence select="if (doc-available($query-uri)) then doc($query-uri) else $error"/>
    <xsl:if test="$debug">
      <xsl:message select="'ds:riResultAll',$query"/>
    </xsl:if>
  </xsl:function>

  <xsl:function name="ds:riQuery">
    <xsl:param name="pid"/>
    <xsl:param name="rel"/>
    <xsl:sequence
      select="concat('select ?o where { &lt;info:fedora/',$pid,'&gt;  &lt;info:fedora/fedora-system:def/view#',$rel,'&gt; ?o }')"
    />
  </xsl:function>

  <xsl:variable name="dateModifiedQuery" select="ds:riQuery($thisPid,'lastModifiedDate')"/>
  <xsl:variable name="dateModified" select="ds:riResultAll($dateModifiedQuery)//sparql:o/text()"/>

</xsl:stylesheet>
