<xsl:stylesheet xmlns="http://www.w3.org/1999/xhtml"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xmlns:sparql="http://www.w3.org/2001/sw/DataAccess/rf1/result"
  xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:mods="http://www.loc.gov/mods/v3" xmlns:ds="http://www.library.tudelft.nl/ns/dataset/"
  xpath-default-namespace="" version="2.0">
  <xsl:import href="/fedora/get/aux:xslt/SETTINGS"/>
  <xsl:output encoding="UTF-8" indent="yes" media-type="text/xml" method="xml" omit-xml-declaration="no" version="1.0"/>
  <xsl:template match="/">
    
    <didl:DIDL xmlns:dcterms="http://purl.org/dc/terms/"
      xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
      xmlns:dii="urn:mpeg:mpeg21:2002:01-DII-NS" xmlns:didl="urn:mpeg:mpeg21:2002:02-DIDL-NS"
      xsi:schemaLocation="urn:mpeg:mpeg21:2002:02-DIDL-NS http://standards.iso.org/ittf/PubliclyAvailableStandards/MPEG-21_schema_files/did/didl.xsd urn:mpeg:mpeg21:2002:01-DII-NS http://standards.iso.org/ittf/PubliclyAvailableStandards/MPEG-21_schema_files/dii/dii.xsd">
      <test>
        <xsl:value-of select="$dateModified"/>
      </test>
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
            <dcterms:modified><xsl:value-of select="$dateModified"/></dcterms:modified>
          </didl:Statement>
        </didl:Descriptor>
        <didl:Component>
          <didl:Resource mimeType="text/html"
            ref="http://resolver.tudelft.nl/{$thisPid}"/>
        </didl:Component>
        <didl:Item>
          <didl:Descriptor>
            <didl:Statement mimeType="application/xml">
              <rdf:type rdf:resource="info:eu-repo/semantics/descriptiveMetadata"/>
            </didl:Statement>
          </didl:Descriptor>
          <didl:Component>
            <didl:Resource mimeType="application/xml">
              <xsl:copy-of select="."/>
            </didl:Resource>
          </didl:Component>
        </didl:Item>
        <didl:Item>
          <didl:Descriptor>
            <didl:Statement mimeType="application/xml">
              <rdf:type rdf:resource="info:eu-repo/semantics/objectFile"/>
            </didl:Statement>
          </didl:Descriptor>
          <didl:Descriptor>
            <didl:Statement mimeType="application/xml">
              <dcterms:accessRights><xsl:text>http://purl.org/eprint/accessRights/OpenAccess</xsl:text></dcterms:accessRights>
            </didl:Statement>
          </didl:Descriptor>
          <didl:Component>
            <didl:Resource mimeType="application/pdf"
              ref="http://repository.tudelft.nl/assets/uuid:86513f19-0e70-4927-bc43-462e154d72dd/otb_bontekoning_20060206.pdf"
            />
          </didl:Component>
        </didl:Item>
        <didl:Item>
          <didl:Descriptor>
            <didl:Statement mimeType="application/xml">
              <rdf:type rdf:resource="info:eu-repo/semantics/humanStartPage"/>
            </didl:Statement>
          </didl:Descriptor>
          <didl:Component>
            <didl:Resource mimeType="text/html"
              ref="http://resolver.tudelft.nl/uuid:86513f19-0e70-4927-bc43-462e154d72dd"/>
          </didl:Component>
        </didl:Item>
      </didl:Item>
    </didl:DIDL>
  </xsl:template>
</xsl:stylesheet>
