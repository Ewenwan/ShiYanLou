// ==UserScript==
// @id             CNKI-PDF-Special
// @name           CNKI 中国知网 PDF 全文下载(特制版)
// @version        1.25
// @namespace      http://yuelong.info
// @author         YUE Long
// @updateURL      https://userscripts.org/scripts/source/164338.meta.js
// @description    ① 点击 CNKI 检索结果界面中的下载按钮可以直接下载 PDF 格式文献；② 在硕士/博士学位论文详细信息界面添加“PDF下载”按钮。 ※特制版功能：在检索页面中的硕士/博士论文默认下载 CAJ 文件。
// @include        http://*.cnki.net/*
// @include        http://*.cnki.net.*/*
// @run-at         document-idle
// ==/UserScript==

var allLis, thisLi, newLi, aPDF, allLinks, thisLink, pageType;
pageType = true;

allLinks = document.evaluate(
	'//a[@href]',
	document,
	null,
	XPathResult.UNORDERED_NODE_SNAPSHOT_TYPE,
	null);

allLis = document.evaluate(
    "//li[@class]",
	document,
	null,
	XPathResult.UNORDERED_NODE_SNAPSHOT_TYPE,
	null);

if (allLis) {
	for (var i = 0; i < allLis.snapshotLength; i++) {
		thisLi = allLis.snapshotItem(i);
		if (thisLi.getAttribute("class").indexOf("readol") !== -1) {
			pageType = false;
			newLi = document.createElement('li');
			newLi.setAttribute("class","pdf");
			aPDF='<a target="_blank" href="' + thisLi.firstChild.href.replace("&dflag=readonline","&dflag=pdfdown") +'">PDF下载</a>';
			newLi.innerHTML=aPDF;
			thisLi.parentNode.insertBefore(newLi, thisLi.nextSibling);
		}
		if (thisLi.getAttribute("class").indexOf("cajNew") !== -1) {
			pageType = false;
		}
	}
}

if (pageType) {
	for (var i = 0; i < allLinks.snapshotLength; i++) {
		thisLink = allLinks.snapshotItem(i);
		if (thisLink.href && thisLink.href.indexOf("download.aspx?filename=") != -1 && thisLink.href.indexOf("&dflag") == -1 && thisLink.href.indexOf("=CMFD") == -1 && thisLink.href.indexOf("=CDFD") == -1 ) {
		thisLink.href = thisLink.href + "&dflag=pdfdown";};
		if (thisLink.href && thisLink.href.indexOf("download.aspx?filename=") != -1 && thisLink.href.indexOf("&dflag") == -1 && (thisLink.href.indexOf("=CMFD") !== -1 || thisLink.href.indexOf("=CDFD") !== -1 )) {
		thisLink.href = thisLink.href + "&dflag=nhdown";};
	}
};