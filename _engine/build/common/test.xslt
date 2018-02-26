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
<div class="title_box"><img class="title" alt="Pixie Engine" src="data:image/png;base64,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
t+1vsGc2dwGOA3pgT3B7EmiW6sbSGIDNs87DMAy7G+idGGnC2DF2lieBmCe7sH+D7a46zhHhu5wN
bBfQ64E/YI8TJEslK+pBlqjRx0wXz45RJW2i8pJTeRiF7d03oZp0U0j8UCISgCbiCezuoIudWn9H
p9Dfij1TeB2JPfnoO0fAyrEDj8XiC2yviOVJcD++wI6/dKFTc/M5tf45zt9HnXTbE/w63sb2Hluf
wIU/wJfOO7CB6j3dxmC7TS9OgudnNPC58/z05OCcke3YHn/rnXc6pVE4aIWDTkbaYXsHHQP8F4kd
okMkMFoRTIjk49+dwh9glswhhARAHD20cprw53DoRJ407IHg4c7/H2HHrhdC1AF1AakLKBG5EvjQ
+bwBewwmhO37Hx34XQ5cjt2/LkSd0JKQEgC9BYlHdw56ZLXloC/3PuxB+WnYcwH2yFRCAiABEEII
EScaAxBCCAmAEEIICYAQQggJgBBCCAmAEEIICYAQQggJgBBCCAmAEEIICYAQQggJgBBCCAmAEEII
CYAQQggJgBBCCAmAEEIICYAQQggJgBBCCAmAEEIICYAQQggJgBBCCAmAEEIICYAQQggJgBBCCAmA
EEIICYAQQqQy/zcAgoPvzE24TAkAAAAASUVORK5CYII=" /></div>
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

