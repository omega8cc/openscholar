<?php

class os_boxes_modal_siteinfo extends os_boxes_modalframe {

  protected function config_path() {
    $vsite = vsite_get_vsite();
    return 'node/'.$vsite->group->nid.'/edit';
  }

  public function options_defaults() {
    return array();
  }

  public function render() {
    $vsite = vsite_get_vsite();
    $block = parent::render();

    // the theme is vsite_site_info regardless of content type
    $block['content'] = theme('vsite_site_info');
    return $block;
  }

  public function form_alter(&$form, &$form_state) {
    if ($form['#id'] = 'node-form' && ($form['form_id']['#value'] == 'vsite_node_form' || $form['form_id']['#value'] == 'project_node_form')) {

      // hide unnecessary fields
      $whitelist = array('title', 'og_description', 'field_vsite_address', 'taxonomy', 'buttons');
      module_load_include('inc', 'spaces_og', 'spaces_og.pages');
      foreach (element_children($form) as $child) {
        if ($form[$child]['#type'] != 'value' && $form[$child]['#type'] != 'hidden') {
          if (!in_array($child, $whitelist)) {
            _spaces_og_make_hidden($form[$child]);
          }
        }
      }
      $form['spaces_preset']['#type'] = 'value';

      // hides the wysiwyg
      /// i do it this way because its a temporary hack
      global $conf;
      $conf['better_formats_allowed_vsite'] = array(2);

      // makes sure the node has a theme
      if (!isset($form['themes'])) {
        $form['theme'] = array(
          '#type' => 'value',
          '#value' => $form['#node']->og_theme,
        );
      }

      if (isset($form['buttons'])) {
        $form['buttons']['#type'] = 'markup';
        $form['buttons']['preview']['#access'] = false;
        $form['buttons']['delete']['#access'] = false;
        $form['buttons']['preview_changes']['#access'] = false;
      }
      parent::form_alter($form, $form_state);
    }
  }

  static public function submit($form, $form_state) {
    // empty on purpose
    // prevents the parent from running
  }
}