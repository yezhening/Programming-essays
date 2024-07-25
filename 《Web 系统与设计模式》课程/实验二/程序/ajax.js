function getXMLHttpRequest() {
    var req = new XMLHttpRequest();
    return req;
}

var http = getXMLHttpRequest();

function getServerTime() {
    var myurl = 'http://211.71.149.251:12705/telltimeXML.php';
    var myRand = parseInt(Math.random() * 999999999999999);
    //add random number to URL to avoid cache problems
    var modurl = myurl + "?rand=" + myRand;
    http.open("GET", modurl, true);
    //set up the callback function
    http.onreadystatechange = useHttpResponse;
    http.send(null);
}

function useHttpResponse() {
    // if (http.readyState == 4) {
        if (http.status == 200) {
            var timeValue = http.responseXML.getElementsByTagName("timenow")[0];
            document.getElementById('showtime').innerHTML = timeValue.childNodes[0].nodeValue;
        // }
    } else {
        document.getElementById('showtime').innerHTML = '<img src="anim.giv">';
    }
}