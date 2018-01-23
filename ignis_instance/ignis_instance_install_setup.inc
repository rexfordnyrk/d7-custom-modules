<?php
/**
 * Created by PhpStorm.
 * User: rexfordnyrk
 * Date: 1/10/18
 * Time: 11:30 PM
 */

class Ignis_Instance_Install_Setup {
  //admin full name
  public $fullname = '';
  //Admin username.
  public $a_username = '';
  //admin password.
  public $a_password = '';
  //admin email address.
  public $email = '';
  //loadconfiguration file;
  public $config_file = null;
  public $basename = null;
  public $command_dir = null;

  /**
   * Ignis_Instance_Install_Setup constructor.
   *
   * @param string $fullname
   * @param string $a_username
   * @param string $a_password
   * @param string $email
   * @param null $config_file
   * @param null $basename
   */
  public function __construct($fullname, $a_username, $a_password, $email, $c_filepath) {
    $this->fullname = $fullname;
    $this->a_username = $a_username;
    $this->a_password = $a_password;
    $this->email = $email;
    $this->config_file = $this->get_config_file_contents($c_filepath);
    $this->basename = basename($c_filepath);
    $this->command_dir = DRUPAL_ROOT.'/'.$c_filepath.'/survey/application/commands/';

  }


  public function get_config_file_contents($c_filepath){

      $filecontents = file(DRUPAL_ROOT.'/'.$c_filepath.'/survey/application/config/config.php');
      return $filecontents;
  }


  public function set_config_file_contents($c_filepath){

    foreach ($this->config_file as $key => $value){
      if (strpos($value, "'tablePrefix' =>") !== false) {
         $this->config_file[$key] = "'tablePrefix' => '$this->basename',";
      }
    }
    $write_file_contents = file_put_contents(DRUPAL_ROOT.'/'.$c_filepath.'/survey/application/config/config.php',
                           $this->config_file);
      return $write_file_contents;
  }

  public function run_instance_install(){
    chdir($this->command_dir);
    $output = shell_exec('php console.php install "'.$this->a_username.'" "'.$this->a_password.'" "'.$this->fullname.'" "'.$this->email.'"');
    (strpos($output,"create-mysql.sql") !== FALSE ? $GLOBALS['install_status'] = "Success" :
      $GLOBALS['install_status'] = "fail:" . $output );
    chdir(DRUPAL_ROOT);
  }

}