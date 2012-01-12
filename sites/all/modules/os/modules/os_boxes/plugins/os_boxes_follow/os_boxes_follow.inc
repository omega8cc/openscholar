<?php

class os_boxes_follow extends os_boxes_default {

  public function options_defaults() {
    return array(
      'links' => array(),
    );
  }

  public function options_form() {
    $form = array();
  	vsite_include('nodeautocomplete');
    $form['#after_build'] = array('os_boxes_follow_add_externals');

    $form['add'] = array(
      '#type' => 'fieldset',
      '#title' => t('Add New Post'),
      'link_to_add' => array(
        '#type' => 'textfield',
        '#title' => t('URL'),
        '#description' => t('Enter the URL of your profile on a social media site, such as Facebook or Twitter.'),
      ),
      'button' => array(
        '#type' => 'markup',
        '#value' => '<input type="button" class="add_new" value="Add" />',
      ),
    );

    // build a normal form
    $form['links'] = array(
      '#type' => 'markup',
      '#tree' => TRUE,
      '#pre_render' => array('os_boxes_follow_table'),
    );

    $links = &$form['links'];
    $this->options['links']['blank'] = array('title' => '__blank__', 'weight' => 2000000); // blank element for copying purposes
    foreach ($this->options['links'] as $k => $v) {
      $n = array(
        '#weight' => $v['weight'],
        'text' => array(
          '#type' => 'markup',
          '#value' => '<span>'.$v['title'].'</span> <a href="#" class="remove" title="Remove Link">Remove</a>',
        ),
        'title' => array(
          '#type' => 'hidden',
          '#default_value' => $v['title'],
        ),
        'weight' => array(
          '#type' => 'textfield',
          '#default_value' => $v['weight'],
          '#size' => 3,
          '#attributes' => array(
            'class' => (string)$k!='blank'?'field-weight':'',
          )
        ),
      );
      $links[$k] = $n;
    }
    uasort($this->options['links'], 'element_sort');

    $form['count'] = array(
      '#type' => 'hidden',
      '#default_value' => count($this->options['links'])-1,
      '#process' => array('os_boxes_follow_add_new_fields'),
    );

    return $form;
  }

  public function render() {
    $block = parent::render();
    $links = array();
    foreach ($this->options['links'] as $n) {
      $l = array(
        'href' => 'node/'.$n['nid'],
        'title' => $n['title'],
      );
      $links[$n['weight']] = $l;
    }
    ksort($links);
    $block['content'] = theme('links', $links);
    return $block;
  }
}

/**
 * Adds the neccessary js to the form
 * @param unknown_type $form
 */
function os_boxes_follow_add_externals($form) {
  drupal_add_js(drupal_get_path('module', 'os_boxes').'/theme/js/os_boxes_follow.js');
  $networks = os_follow_default_networks();
  drupal_alter('os_follow_networks', $networks);
  drupal_add_js(array('follow_networks' => $networks), 'setting');

  return $form;
}

/**
 * Renders the list of links as a table.
 * @param unknown_type $element
 */
function os_boxes_follow_table($element) {
  // render the form as a table
  $rows = array();
  uasort($element, 'element_sort');
  foreach (element_children($element) as $c) {
    $rows[] = array(
      'data' => array(
        drupal_render($element[$c]['text']).drupal_render($element[$c]['title']),
        drupal_render($element[$c]['weight']),
      ),
      'class' => (string)$c!='blank'?'draggable':'',
    );
  }
  $element['#value'] = theme('table', array(), $rows, array('id' => 'follow-links-list'));
  drupal_add_tabledrag('follow-links-list', 'order', 'sibling', 'field-weight');
  return $element;
}

/**
 * Adds new elements to the form array so their values appear in form_state
 *
 * When we get this form back, it will have some fields added through js that weren't there
 * before. In here, we rebuild that section of the form array, and pass it through form builder
 * so form_state[values] is populated with our new fields
 * @param unknown_type $element
 * @param unknown_type $post
 * @param unknown_type $form_state
 * @param unknown_type $form
 */
function os_boxes_follow_add_new_fields(&$element, $post, &$form_state, &$form) {
  for ($i=$element['#default_value']; $i < $post['count']; $i++) {
    $form['options']['links'][$i] = $form['options']['links']['blank'];
    $form['options']['links'][$i] += array(
      '#attributes' => array('id' => $i),
      '#parents' => array('links', $i),
      '#array_parents' => array('options', 'links', $i),
      '#tree' => TRUE,
      '#post' => $element['#post'],
    );
    form_builder($form['form_id']['#value'], $form['options']['links'][$i], $form_state);
  }
  unset($form_state['values']['links']['blank']);

  // remove nids that aren't in the post
  foreach ($form_state['values']['links'] as $k => $v) {
    if (!isset($form_state['input']['links'][$k])) {
      unset($form_state['values']['links'][$k]);
    }
  }

  return $element;
}

/**
 * Retrieves the default networks available.
 * Copied from Follow module
 *
 * @return
 *   An associative array, keyed by the machine name. The values are an array
 *   including title of the network, along with the domain to be used for
 *   input validation of the network.
 */
function os_follow_default_networks() {
  $networks = array(
    'facebook'  => array(
      'title' => t('Facebook'),
      'domain' => 'facebook.com',
    ),
    'virb'      => array(
      'title' => t('Virb'),
      'domain' => 'virb.com',
    ),
    'myspace'   => array(
      'title' => t('MySpace'),
      'domain' => 'myspace.com',
    ),
    'twitter'   => array(
      'title' => t('Twitter'),
      'domain' => 'twitter.com',
    ),
    'picasa'    => array(
      'title' => t('Picasa'),
      'domain' => 'picasaweb.google.com',
    ),
    'flickr'    => array(
      'title' => t('Flickr'),
      'domain' => 'flickr.com',
    ),
    'youtube'   => array(
      'title' => t('YouTube'),
      'domain' => 'youtube.com',
    ),
    'vimeo'     => array(
      'title' => t('Vimeo'),
      'domain' => 'vimeo.com',
    ),
    'bliptv'    => array(
      'title' => t('blip.tv'),
      'domain' => 'blip.tv',
    ),
    'lastfm'    => array(
      'title' => t('last.fm'),
      'domain' => 'last.fm',
    ),
    'linkedin'  => array(
      'title' => t('LinkedIn'),
      'domain' => 'linkedin.com',
    ),
    'delicious' => array(
      'title' => t('Delicious'),
      'domain' => 'delicious.com',
    ),
    'tumblr'    => array(
      'title' => t('Tumblr'),
      'domain' => 'tumblr.com',
    ),
  );

  return $networks;
}