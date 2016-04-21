<xsl:stylesheet xmlns="http://www.w3.org/1999/xhtml" xmlns:ds="http://www.library.tudelft.nl/ns/dataset/" xmlns:sparql="http://www.w3.org/2001/sw/DataAccess/rf1/result"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:mods="http://www.loc.gov/mods/v3"
  xpath-default-namespace="" version="2.0">

  <!-- on the server href should be: "/fedora/get/aux:xslt/MODS2MODS" 
  <xsl:import href="/fedora/get/aux:xslt/MODS2MODS"/> -->
  <xsl:import href="islandora_proai_xslt_mods2mods.xsl"/>

  <xsl:output encoding="UTF-8" indent="yes" media-type="text/xml" method="xml" omit-xml-declaration="no" version="1.0"/>

  <xsl:template match="/">
    <didl:DIDL xmlns:dcterms="http://purl.org/dc/terms/" xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" xmlns:dii="urn:mpeg:mpeg21:2002:01-DII-NS" xmlns:didl="urn:mpeg:mpeg21:2002:02-DIDL-NS"
      xsi:schemaLocation="urn:mpeg:mpeg21:2002:02-DIDL-NS http://standards.iso.org/ittf/PubliclyAvailableStandards/MPEG-21_schema_files/did/didl.xsd urn:mpeg:mpeg21:2002:01-DII-NS http://standards.iso.org/ittf/PubliclyAvailableStandards/MPEG-21_schema_files/dii/dii.xsd">
      <didl:Item>
        <didl:Descriptor>
          <didl:Statement mimeType="application/xml">
            <dii:Identifier>
              <xsl:value-of select="$urnNBNTudelft"/>
              <xsl:value-of select="$thisPid"/>
            </dii:Identifier>
          </didl:Statement>
        </didl:Descriptor>
        <didl:Descriptor>
          <didl:Statement mimeType="application/xml">
            <dcterms:modified>
              <xsl:value-of select="$dateModified"/>
            </dcterms:modified>
          </didl:Statement>
        </didl:Descriptor>
        <didl:Component>
          <didl:Resource mimeType="text/html" ref="{$resolver}{$thisPid}"/>
        </didl:Component>

        <didl:Item>
          <didl:Descriptor>
            <didl:Statement mimeType="application/xml">
              <rdf:type rdf:resource="info:eu-repo/semantics/descriptiveMetadata"/>
            </didl:Statement>
          </didl:Descriptor>
          <didl:Component>
            <didl:Resource mimeType="application/xml">
              <xsl:apply-templates select="mods:mods"/>
            </didl:Resource>
          </didl:Component>
        </didl:Item>
        
        <!-- process datastreams -->
        <xsl:for-each select="$dsList">
          <xsl:variable name="objExtension" select="tokenize(./@uri,'/')[last()]"/>

          <xsl:variable name="activeQuery" select="ds:riQuery(./@uri,'info:fedora/fedora-system:def/model#state')"/>
          <xsl:variable name="activeResult" select="ds:riResultAll($activeQuery)//sparql:o/@uri"/>
          <xsl:variable name="isActive" select="tokenize($activeResult,'#')[last()] = 'Active'"/>

          <xsl:variable name="embargoQuery" select="ds:riQuery(./@uri,'info:islandora/islandora-system:def/scholar#embargo-until')"/>
          <xsl:variable name="embargoDate" select="ds:riResultAll($embargoQuery)//sparql:o/text()"/>
          <xsl:variable name="hasEmbargo" select="current-dateTime() &lt; $embargoDate"/>

          <xsl:if test="$isActive">
            <didl:Item>
              <didl:Descriptor>
                <didl:Statement mimeType="application/xml">
                  <rdf:type rdf:resource="info:eu-repo/semantics/objectFile"/>
                </didl:Statement>
              </didl:Descriptor>
              <didl:Descriptor>
                <didl:Statement mimeType="application/xml">
                  <dcterms:accessRights>
                    <xsl:choose>
                      <xsl:when test="$hasEmbargo">
                        <xsl:text>http://purl.org/eprint/accessRights/RestrictedAccess</xsl:text>
                      </xsl:when>
                      <xsl:otherwise>
                        <xsl:text>http://purl.org/eprint/accessRights/OpenAccess</xsl:text>
                      </xsl:otherwise>
                    </xsl:choose>
                  </dcterms:accessRights>
                </didl:Statement>
              </didl:Descriptor>
              <didl:Component>
                <didl:Resource mimeType="application/pdf" ref="{$download1}{$thisPid}{$download2}{$objExtension}{$download3}"/>
              </didl:Component>
            </didl:Item>
          </xsl:if>
        </xsl:for-each>

        <didl:Item>
          <didl:Descriptor>
            <didl:Statement mimeType="application/xml">
              <rdf:type rdf:resource="info:eu-repo/semantics/humanStartPage"/>
            </didl:Statement>
          </didl:Descriptor>
          <didl:Component>
            <didl:Resource mimeType="text/html" ref="{$resolver}{$thisPid}"/>
          </didl:Component>
        </didl:Item>
      </didl:Item>
    </didl:DIDL>
  </xsl:template>
</xsl:stylesheet>
