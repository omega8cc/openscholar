// The following line will prevent a JavaScript error if this file is included and vertical tabs are disabled.
Drupal.verticalTabs = Drupal.verticalTabs || {};

Drupal.verticalTabs.cite_distribute_selections = function() {
  var vals = [];
  if ($('#edit-googlescholar-meta-module').attr('checked')) {
    vals.push(Drupal.t('Google Scholar'));
  }
  if (vals.join(', ') == '') {
    return Drupal.t('None');
  }
  if ($('#edit-repec-meta-module').attr('checked')) {
    vals.push(Drupal.t('RePEc'));
  }
  else {
    vals.push(Drupal.t(''));
  }
  return vals.join(' ');
}