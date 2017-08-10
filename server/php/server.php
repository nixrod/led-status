<?php

// get the HTTP method, path and body of the request
$method = $_SERVER['REQUEST_METHOD'];
$input = json_decode(file_get_contents('php://input'),true);

// connect to the mysql database
$link = mysqli_connect('localhost', '<mysql_user>', '<mysql_pw>', '<db_name>');
mysqli_set_charset($link,'utf8');
// Check connection
if (!$link) {
    die("Connection failed: " . mysqli_connect_error());
}

// Check if table needs to be created
$sqlCheckTable = "SELECT 1 FROM arduino_led LIMIT 1";
if (!mysqli_query($link, $sqlCheckTable)) {
    initDBTable($link);
}

// Handler for GET request
if ($method == "GET") {
  $sql = "SELECT * FROM arduino_led";
  $result = mysqli_query($link, $sql);

  $rows = array();
  while($row = mysqli_fetch_array($result, MYSQLI_ASSOC)){
    $rows[$row['type']] = boolval($row['isActive']);
  }

  header('Content-Type: application/json');
  echo json_encode($rows);
}

// Handler for POST request
if ($method == "POST") {
  if (array_key_exists("led", $input) && is_bool($input["led"])) {
    $sql = "";
    if ($input["led"]) {
      $sql = "UPDATE arduino_led SET isActive=true WHERE id=1";
    } else {
      $sql = "UPDATE arduino_led SET isActive=false WHERE id=1";
    }
    mysqli_query($link, $sql);
  }

if (array_key_exists("virtualLed", $input) && is_bool($input["virtualLed"])) {
    $sql = "";
    if ($input["virtualLed"]) {
      $sql = "UPDATE arduino_led SET isActive=true WHERE id=2";
    } else {
      $sql = "UPDATE arduino_led SET isActive=false WHERE id=2";
    }
    mysqli_query($link, $sql);
  }

  echo "status updated";
}


function initDBTable($link) {
  // sql to create arduino table
  $sql = "CREATE TABLE arduino_led (
  id INT(6) AUTO_INCREMENT PRIMARY KEY,
  type VARCHAR(30) NOT NULL,
  isActive BOOLEAN NOT NULL
  );";

  $sql .= "INSERT INTO arduino_led (type, isActive)
  VALUES ('led', false);";
  $sql .= "INSERT INTO arduino_led (type, isActive)
  VALUES ('virtualLed', false);";

  if (!mysqli_multi_query($link, $sql)) {
      echo "Error: " . $sql . "<br>" . mysqli_error($link);
  }
}
