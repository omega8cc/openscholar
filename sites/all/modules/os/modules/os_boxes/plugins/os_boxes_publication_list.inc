<?php

/**
 * @file
 * The class for "Listing Publications by Type" widget.
 */

class os_boxes_publication_list extends os_boxes_default {
  
  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    $options = parent::options_defaults();

    $options['pub_types'] = array();
    $options['number_of_items'] = 6;
    $options['more'] = 0;
    $options['more_text'] = t('more');
    $options['more_link'] = "";
    $options['use_pager'] = 1;

    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(){
    
  	ctools_include('dependent');
    ctools_add_js('dependent');
    
    $q = db_query("SELECT tid, name FROM {biblio_types} WHERE tid > 0 AND visible = 1");
    $options = array();
    while ($a = db_fetch_array($q)) $options[$a['tid']] = $a['name'];

    //Set to all types if none have been selected
  	if(!count($this->options['pub_types'])) $this->options['pub_types'] = array_keys($options);

  	$form['number_of_items'] = array(
      '#type' => 'textfield',
      '#title' => 'Number of displayed posts',
      '#default_value' => $this->options['number_of_items'],
      '#size' => 10
    );
    
    $form['use_pager'] = array(
      '#title' => t('Add pager'),
      '#type' => 'checkbox',
      '#default_value' => $this->options['use_pager'],
      '#description' => t('Will add a pager at the end of the list')
    );
    
    $form['more'] = array(
      '#type' => 'checkbox',
      '#title' => t('Add "more" link'),
      '#description' => t('This will add a more link to the bottom of this widget, which will link to the page listing all the posts of type selected above'),
      '#default_value' => $this->options['more'],
      '#tree' => TRUE
    );
    
    $form['more_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Label'),
      '#default_value' => $this->options['more_text'],
      '#description' => t("The title / label of the link."),
      '#process' => array(
        'ctools_dependent_process'
      ),
      '#dependency' => array(
        'edit-more' => array(
          1
        )
      )
    );
    
    $form['more_link'] = array(
      '#type' => 'textfield',
      '#title' => t('Link'),
      '#default_value' => $this->options['more_link'],
      '#description' => t("The url/path for this link."),
      '#process' => array(
        'ctools_dependent_process'
      ),
      '#dependency' => array(
        'edit-more' => array(
          1
        )
      )
    );
    
    $form['pub_types'] = array(
      '#type' => 'checkboxes',
      '#options' => $options,
      '#default_value' => $this->options['pub_types'],
      '#title' => t('Show the following types'),
      '#attributes' => array('class' => 'pub_type'),
      '#description' => t('Select the publication types you want to appear in the widget.')
    );
    
    $form += parent::options_form();
    return $form;
  }
  
  /**
   * Custom submit hander to perform post_submit actions
   */
  public static function options_validate(&$form,&$form_state){
  	
    if(array_keys($form_state['values']['pub_types']) == array_keys($form['options']['pub_types']['#options'])){
    	$form_state['values']['pub_types'] = array();
    }//All selected is the same as no filter, set to none selected
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {
    //@todo
    $block = parent::render();

    $view_name = 'publication_types';
    $display_id = "page_1";
    
    if ($view = views_get_view($view_name)) {
      if ($view->access($display_id)) {
        $view->set_display($display_id);
        
        //Tie this view to this box (this tells the view to call this box's view alter)
        $view->dom_id = "boxes-".$this->delta;
        
        $output = $view->execute_display();
        $block['content'] = $output;
      }
      $view->destroy();
    }

    return $block;
  }

  /**
   * Called from views_pre_view
   *
   * @param view $view
   */
  public function views_alter(&$view, &$display_id, &$args){
  	
  	if (count($this->options['pub_types'])) {
      $filter = array(
        'operator' => 'in',
        'value' => $this->options['pub_types'],
        'group' => '0',
        'exposed' => FALSE,
        'expose' => array(
          'operator' => FALSE,
          'label' => '',
        ),
        'id' => 'tid',
        'table' => 'biblio_types',
        'field' => 'tid',
        'override' => array(
          'button' => 'Use default',
        ),
        'relationship' => 'none',
      );
      $view->set_item($display_id, 'filter', 'tid', $filter);
    }
                
    //Set pager up
    if($this->options['use_pager']){
	    $view->display_handler->set_option('use_pager', 'mini');
	    $view->display_handler->set_option('use_ajax', TRUE);
    }else{
    	$view->display_handler->set_option('use_pager', false);
      $view->display_handler->set_option('use_ajax', false);
    }
    
    if($this->options['more']){
    	//ADD THE MORE TEXT
    	$view->display_handler->set_option('use_more', true);
    	$view->display_handler->set_option('use_more_text', $this->options['more_text']);
    	$view->display_handler->set_option('path', $this->options['more_link']);
    }
    
    //Set the number of items
    if(intval($this->options['number_of_items'])){
    	$view->display_handler->set_option('items_per_page',intval($this->options['number_of_items']));
    	$view->set_items_per_page(intval($this->options['number_of_items']));
    }
  }
  
  /**
   * Return the cache parameters for this box
   */
  public function cache_info(){

//    //Clear when biblio nodes are saved
//    $cache_key = array(
//      'os_og' => array('current' => 'current'),
//      'node' => array('biblio' => 'biblio'),
//    );
//
//    return array('type' => BLOCK_CACHE_CONTENT_CACHE, 'keys' => $cache_key);

    return array(); //caching disabled so ajax pager gets init'ed
  }

}