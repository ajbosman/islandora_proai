<xsl:stylesheet
  xmlns="http://www.w3.org/1999/xhtml"
  xmlns:ds="http://www.library.tudelft.nl/ns/dataset/"
  xmlns:sparql="http://www.w3.org/2001/sw/DataAccess/rf1/result"
  xmlns:xs="http://www.w3.org/2001/XMLSchema"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  exclude-result-prefixes="#all"
  version="2.0"
  xpath-default-namespace="">
  <xsl:template match="/">
    <xsl:text>MODS2DIDL</xsl:text>
    <xsl:copy-of select="."/>
  </xsl:template>
</xsl:stylesheet>