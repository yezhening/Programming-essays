<?php
   header('Content-Type:text/xml');
   sleep(3);
   echo "<?xml  version=\"1.0\" ?><clock1><timenow>".date('H-i-s')."</timenow></clock1>";