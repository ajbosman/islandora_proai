<xsl:stylesheet xmlns="http://www.w3.org/1999/xhtml" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:mods="http://www.loc.gov/mods/v3" exclude-result-prefixes="#all" version="2.0" xpath-default-namespace="">

  <!-- on the server href should be: "/fedora/get/aux:xslt/SETTINGS" 
  <xsl:import href="/fedora/get/aux:xslt/SETTINGS"/> -->
  <xsl:import href="islandora_proai_xslt_settings.xsl"/>

  <xsl:output encoding="UTF-8" indent="yes" media-type="text/xml" method="xml" omit-xml-declaration="no" version="1.0"/>

  <!-- identity template: -->
  <xsl:template match="@*|node()">
    <xsl:copy>
      <xsl:apply-templates select="@*|node()"/>
    </xsl:copy>
  </xsl:template>

  <xsl:template match="mods:mods/mods:identifier[@type='uri'][1]">
    <xsl:copy>
      <xsl:apply-templates select="@*|node()"/>
    </xsl:copy>
    <mods:location>
      <mods:url>
        <xsl:value-of select="$resolver"/>
        <xsl:value-of select="./mods:identifier[@type='uri'][1]"/>
      </mods:url>
    </mods:location>
  </xsl:template>

  <xsl:template match="/mods:mods/mods:name/mods:role">
    <xsl:choose>
      <xsl:when test="$st='didl'">
        <mods:role>
          <mods:roleTerm type="code" authority="marcrelator">
            <xsl:apply-templates select="./mods:roleTerm"/>
          </mods:roleTerm>
        </mods:role>
      </xsl:when>
      <xsl:otherwise>
        <xsl:copy-of select="."/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="/mods:mods/mods:name/mods:role/mods:roleTerm">
    <xsl:choose>
      <xsl:when test="text()='author'"><xsl:text>aut</xsl:text></xsl:when>
      <xsl:when test="text()='corporate'"><xsl:text>aut</xsl:text></xsl:when>
      <xsl:when test="text()='editor'"><xsl:text>edt</xsl:text></xsl:when>
      <xsl:when test="text()='advisor'"><xsl:text>ths</xsl:text></xsl:when>
      <xsl:when test="text()='researcher'"><xsl:text>res</xsl:text></xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="text()"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="/mods:mods/mods:genre">
    <xsl:choose>
      <xsl:when test="$st='didl'">
        <mods:genre>
          <xsl:value-of select="$genrePrefix"/>
          <xsl:choose>
            <xsl:when test="lower-case(replace(text(),'\s+',''))='article'"><xsl:text>article</xsl:text></xsl:when>
            <xsl:when test="lower-case(replace(text(),'\s+',''))='bachelorthesis'"><xsl:text>bachelorThesis</xsl:text></xsl:when>
            <xsl:when test="lower-case(replace(text(),'\s+',''))='book'"><xsl:text>book</xsl:text></xsl:when>
            <xsl:when test="lower-case(replace(text(),'\s+',''))='bookchapter'"><xsl:text>bookPart</xsl:text></xsl:when>
            <xsl:when test="lower-case(replace(text(),'\s+',''))='bookpart'"><xsl:text>bookPart</xsl:text></xsl:when>
            <xsl:when test="lower-case(replace(text(),'\s+',''))='conferencepaper'"><xsl:text>conferencePaper</xsl:text></xsl:when>
            <xsl:when test="lower-case(replace(text(),'\s+',''))='doctoralthesis'"><xsl:text>doctoralThesis</xsl:text></xsl:when>
            <xsl:when test="lower-case(replace(text(),'\s+',''))='journalarticle'"><xsl:text>article</xsl:text></xsl:when>
            <xsl:when test="lower-case(replace(text(),'\s+',''))='lecturenote'"><xsl:text>lecture</xsl:text></xsl:when>
            <xsl:when test="lower-case(replace(text(),'\s+',''))='lecturenotes'"><xsl:text>lecture</xsl:text></xsl:when>
            <xsl:when test="lower-case(replace(text(),'\s+',''))='masterthesis'"><xsl:text>masterThesis</xsl:text></xsl:when>
            <xsl:when test="lower-case(replace(text(),'\s+',''))='patent'"><xsl:text>patent</xsl:text></xsl:when>
            <xsl:when test="lower-case(replace(text(),'\s+',''))='publiclecture'"><xsl:text>lecture</xsl:text></xsl:when>
            <xsl:when test="lower-case(replace(text(),'\s+',''))='report'"><xsl:text>report</xsl:text></xsl:when>
            <xsl:when test="lower-case(replace(text(),'\s+',''))='studentreport'"><xsl:text>report</xsl:text></xsl:when>
            <xsl:when test="lower-case(replace(text(),'\s+',''))='technicaldrawing'"><xsl:text>technicalDocumentation</xsl:text></xsl:when>
            <xsl:when test="lower-case(replace(text(),'\s+',''))='workingpaper'"><xsl:text>workingPaper</xsl:text></xsl:when>
            <xsl:otherwise>other</xsl:otherwise>
          </xsl:choose>
        </mods:genre>
      </xsl:when>
      <xsl:otherwise>
        <xsl:copy-of select="."/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="mods:mods/relatedItem[@type='constituent']"/>


</xsl:stylesheet>
