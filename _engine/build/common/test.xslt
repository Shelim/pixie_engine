<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:template match="/">
<html>
<head>
<meta charset="UTF-8" />
<title>The Pixie Engine test result</title>
<style>
.title_box
{
    text-align:center;
}
.title
{
    text-align:center;
}
hr
{
    border-style:outset;
    border-width:3px;
    width:80%;
    text-align:center;
}
h1, h2, h3
{
    text-align:center;
}
h3
{
    font-weight:bold;
    font-style:italic;
}
.copyrights
{
    text-align:center;
    margin-top:40px;
    font-style:italic;
}
.success_text
{
    color:#2D8633;
}
.failure_text
{
    color:#9E354A;
}
.heading
{
    page-break-before: always;
}
table
{
    width:70%;
    margin-top:20px;
    margin-left:auto;
    margin-right:auto;
    border-collapse: collapse; 
	border:none;
    text-align:center;
	border-left:2px solid black;
	border-right:2px solid black;

}

tr
{
	border-top:2px solid black;
	border-bottom:2px solid black;
}
td
{
    padding:10px;
}
.success_box
{
    background-color:#54A759;
}

.success_box:hover
{
    background-color:#11FF11 !important;
    color:#33333 !important;
}

.failure_box
{
    color:#BBBBBB !important;
    background-color:#4F0010;
    font-weight:bold;
}

.failure_box:hover
{
    color:#FFFFFF !important;
    font-weight:bold !important;
    background-color:#AA1111 !important;
    border-color:#FF5555;
}

.failure_hr
{
    border-style:dashed;
    border-width:1px;
    border-color:#FF0000;
}
@media print
{
    .noprint
    {
        display: none;
    }
    .failure_box
    {
        color:#9E354A; !important;
    }
    .success_box
    {
        color:#2D8633; !important;
    }
}

</style>
</head>
<body>
<div class="title_box"><img class="title" alt="Pixie Engine" src="data:image/png;base64,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" /></div>
<hr />
<xsl:if test="testsuites/@failures=0">
<h1 class="success_text">Unit test raport</h1>
<h2 class="success_text">Run at <code><xsl:value-of select="substring-before(testsuites/@timestamp, 'T')"/></code>, <code><xsl:value-of select="substring-after(testsuites/@timestamp, 'T')"/></code> (execution time <code><xsl:value-of select="testsuites/@time" /></code> seconds)</h2>
<h3 class="success_text">Success</h3>
</xsl:if>
<xsl:if test="testsuites/@failures&gt;0">
<h1 class="failure_text">Unit test raport</h1>
<h2 class="failure_text">Run at <code><xsl:value-of select="substring-before(testsuites/@timestamp, 'T')"/></code>, <code><xsl:value-of select="substring-after(testsuites/@timestamp, 'T')"/></code> (execution time <code><xsl:value-of select="testsuites/@time" /></code> seconds)</h2>
<h1 class="failure_text">FAILURE: Failed <code><xsl:value-of select="testsuites/@failures" /></code> out of <code><xsl:value-of select="testsuites/@tests" /></code></h1>
</xsl:if>
<hr />
<div class="copyrights">Copyrights &#169; <b>2016 - 2018</b> Kosek.com</div>
<xsl:if test="testsuites/@failures&gt;0">
<h1 class="heading failure_text">Failed tests</h1>
<hr />
<table><xsl:for-each select="testsuites/testsuite">
<xsl:for-each select="./testcase">
<xsl:if test="./failure">
<tr class="failure_box"><td><code><xsl:value-of select="./@classname" /></code> -> <code><xsl:value-of select="./@name" /></code><hr class="failure_hr" />
<code><xsl:value-of select="./failure/@message" /></code></td><td>FAILURE (<code><xsl:value-of select="./@time" /></code> seconds)</td></tr>
</xsl:if>
</xsl:for-each>
</xsl:for-each>
</table>
</xsl:if>
<h1 class="heading success_text">Succeded tests</h1>
<hr />
<table>
<xsl:for-each select="testsuites/testsuite">
<xsl:for-each select="./testcase">
<xsl:if test="not(./failure)">
<tr class="success_box"><td><code><xsl:value-of select="./@classname" /></code> -> <code><xsl:value-of select="./@name" /></code></td><td>OK (<code><xsl:value-of select="./@time" /></code> seconds)</td></tr>
</xsl:if>
</xsl:for-each>
</xsl:for-each>
</table>
</body>
</html>
</xsl:template>
</xsl:stylesheet>

