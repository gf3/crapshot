var i;
var sheet;

for (i = 0; i < document.styleSheets.length; i++) {
  sheet = document.styleSheets[i];

  if (sheet.media && sheet.media.length == 1) {
    // TODO: Fragile
    switch (sheet.media[0]) {
      case 'print':
        sheet.ownerNode.parentNode.removeChild(sheet.ownerNode);
        break;

      case 'screen':
        sheet.ownerNode.media = 'all';
        break;
    }
  }
}

for (i = 0; i < document.styleSheets.length; i++) {
  if (document.styleSheets[i].cssRules) {
    deletePrintStyles(document.styleSheets[i].cssRules);
  }
}

function deletePrintStyles(ruleList) {
  var i;
  var rule;

  for (i = 0; i < ruleList.length; i++) {
    rule = ruleList[i];

    if (rule.type === CSSRule.MEDIA_RULE && rule.media.length === 1 && rule.media[0] === 'print') {
      if (rule.parentRule) {
        rule.parentRule.deleteRule(i);
      }
      else {
        rule.parentStyleSheet.deleteRule(i);
      }

      continue;
    }

    if (rule.cssRules) {
      deletePrintStyles(rule.cssRules);
    }
  }
}
