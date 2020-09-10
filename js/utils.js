function http_get(url) {
  var xmlHttp = new XMLHttpRequest();
  xmlHttp.open("GET", url, false); // false for synchronous request
  xmlHttp.send(null);
  return xmlHttp.responseText;
}

function http_post(url, data, cb) {
  var xhr = new XMLHttpRequest();
  xhr.open("POST", url, true);
  xhr.setRequestHeader("Content-Type", "application/json");
  xhr.onreadystatechange = function () {
    if (this.readyState != 4) return;
    if (cb) {
      cb(this.status, this.responseText);
    }
  };

  var msg = JSON.stringify(data);
  xhr.send(msg);
}
