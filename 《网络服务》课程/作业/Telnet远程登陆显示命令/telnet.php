<?php
class Telnet
{
    private $host;
    private $port;
    private $timeout; //设置连接的时限，单位为秒。

    private $socket = NULL;
    private $buffer = NULL;
    private $prompt;
    private $errno; //指定在系统执行connect（）调用中发生的系统级错误号
    private $errstr; //错误信息将以字符串的信息返回。

    private $NULL;
    private $DC1;
    private $WILL;
    private $WONT;
    private $DO;
    private $DONT;
    private $IAC;

    private $global_buffer = '';
    const TELNET_ERROR = FALSE; //static properies
    const TELNET_OK = TRUE; //static properies

    public function __construct($host = '127.0.0.1', $port = '23', $timeout = 10) //构造函数是" __"
    {
        $this->host = $host;
        $this->port = $port;
        $this->timeout = $timeout;

        //set some telnet special characters
        //chr() 函数从指定的 ASCII 值返回字符
        $this->NULL = chr(0); //NUL 空字符（Null）
        $this->DC1 = chr(17); //DC1 设备控制一（XON 启用软件速度控制）
        $this->WILL = chr(251); //√
        $this->WONT = chr(252); //?
        $this->DO = chr(253); //&sup2
        $this->DONT = chr(254); //■
        $this->IAC = chr(255); //?

        $this->connect();
    } // __construct

    public function __destruct()
    {
        //clean up resources
        $this->disconnect();
        $this->buffer = NULL;
        $this->global_buffer = NULL;
    } // __desstruct

    public function connect()
    {
        //check if we need to convert host to IP
        //preg_match 函数用于执行一个正则表达式匹配
        if (!preg_match('/([0-9]{1,3}\\.){3,3}[0-9]{1,3}/', $this->host)) {
            $ip = gethostbyname($this->host);
            if ($this->host == $ip) {
                throw new Exception("Cannot resolve $this->host");
            } else {
                $this->host = $ip;
            }
        }
        $this->socket = fsockopen($this->host, $this->port, $this->errno, $this->errstr, $this->timeout);
        if (!$this->socket) {
            throw new Exception("cannot connect to $this->host on port $this->port");
        }
        return self::TELNET_OK;
    } //connect()

    public function disconnect()
    {
        if ($this->socket) {
            if (!fclose($this->socket)) {
                throw new Exception("Error while closing telnet socket");
            }
            $this->socket = NULL;
        }
        return self::TELNET_OK;
    } //disconnect

    public function exec($command)
    {
        $this->write($command);
        $this->waitPrompt();
        return $this->getBuffer();
    } //exec()

    public function login($username, $password)
    {
        try {
            $this->setPrompt('login:');
            $this->waitPrompt();
            $this->write($username);
            $this->setPrompt('Password:');
            $this->waitPrompt();
            $this->write($password);
            $this->setPrompt();
            $this->waitPrompt();
        } catch (Exception $e) {
            throw new Exception("Login failed");
        }
        return self::TELNET_OK;
    } //login()

    public function setPrompt($s = '$')
    {
        $this->prompt = $s;
        return self::TELNET_OK;
    } //setPrompt()

    protected function getc()
    {
        $c = fgetc($this->socket);
        $this->global_buffer .= $c;
        return $c;
    } //getc()

    protected function clearBuffer()
    {
        $this->buffer = '';
    }

    protected function readTo($prompt)
    {
        if (!$this->socket) {
            throw new Exception("Telnet connection closed");
        }
        //clear the buffer
        $this->clearBuffer();
        $until_t = time() + $this->timeout;
        do {
            if (time() > $until_t) {
                throw new Exception("couldn't find the requested:'$prompt' within {$this->timeout} seconds");
            }
            $c = $this->getc();
            if ($c === false) {
                throw new Exception("couldn't find the requested:'" . $prompt . "',it was not in the data returned from server:" . $this->buffer);
            }
            if ($c === $this->IAC) {
                if ($this->negotiateTelnetOptions()) {
                    continue;
                }
            }
            $this->buffer .= $c;
            if ((substr($this->buffer, strlen($this->buffer) - strlen($prompt))) == $prompt) {
                return self::TELNET_OK;
            }
        } //do
        while ($c != $this->NULL || $c != $this->DC1);
    } //readTo()

    protected function write($buffer, $addNewLine = true)
    {
        if (!$this->socket) {
            throw new Exception("Telnet connection closed");
        }
        $this->clearBuffer();
        if ($addNewLine == true) {
            $buffer .= "\n";
        }
        $this->global_buffer .= $buffer;
        if (!fwrite($this->socket, $buffer) < 0) {
            throw new Exception("Error writing to socket");
        }
        return self::TELNET_OK;
    } //write()

    public function getBuffer()
    {
        $buf = explode("\n", $this->buffer);
        unset($buf[count($buf) - 1]);
        $buf = implode("\n", $buf);
        return trim($buf);
    } //getBuffer()

    public function getGlocalBuffer()
    {
        return $this->global_buffer;
    } //getGlocalBuffer()

    protected function negotiateTelnetOptions()
    {
        $c = $this->getc();
        if ($c != $this->IAC) {
            if (($c == $this->DO) || ($c == $this->DONT)) {
                $opt = $this->getc();
                fwrite($this->socket, $this->IAC . $this->WONT . $opt);
            } else if (($c == $this->WILL) || ($c == $this->WONT)) {
                $opt = $this->getc();
                fwrite($this->socket, $this->IAC . $this->DONT . $opt);
            } else {
                throw new Exception('Error:unlnown control character' . ord($c));
            }
        } else {
            throw new Exception('Error:Something Wicked Happened');
        }
        return self::TELNET_OK;
    } //negotiateTelnetOptions()

    protected function waitPrompt()
    {
        return $this->readTo($this->prompt);
    } //waitPrompt()
}//class
