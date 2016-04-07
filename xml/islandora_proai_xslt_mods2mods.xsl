<xsl:stylesheet
  xmlns="http://www.w3.org/1999/xhtml"
  xmlns:ds="http://www.library.tudelft.nl/ns/dataset/"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xmlns:sparql="http://www.w3.org/2001/sw/DataAccess/rf1/result"
  xmlns:xs="http://www.w3.org/2001/XMLSchema"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  exclude-result-prefixes="#all"
  version="2.0"
  xpath-default-namespace="">
  <xsl:import href="/fedora/get/aux:xslt/SETTINGS"/>
  <xsl:output encoding="UTF-8" indent="yes" media-type="text/xml" method="xml" omit-xml-declaration="no" version="1.0"/>

  <xsl:template match="/">
    <xsl:copy-of select="."/>
  </xsl:template>
</xsl:stylesheet>