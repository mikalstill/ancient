#!/usr/bin/python

# With many thanks to the documentation from
#   http://wiki.samygo.tv/index.php5/MessageBoxService_request_format

import httplib

url = 'http://192.168.1.103:52235/PMR/control/MessageBoxService'
msg = """<?xml version="1.0" encoding="utf-8"?>
<s:Envelope s:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/" xmlns:s="http://schemas.xmlsoap.org/soap/envelope/" >
  <s:Body>
    <u:AddMessage xmlns:u="urn:samsung.com:service:MessageBoxService:1\">
      <MessageType>text/xml</MessageType>
      <MessageID>can be anything</MessageID>
      <Message>
        &lt;Category&gt;SMS&lt;/Category&gt;
        &lt;DisplayType&gt;Maximum&lt;/DisplayType&gt;
        &lt;ReceiveTime&gt;
        &lt;Date&gt;2010-05-04&lt;/Date&gt;
        &lt;Time&gt;01:02:03&lt;/Time&gt;
        &lt;/ReceiveTime&gt;
        &lt;Receiver&gt;
        &lt;Number&gt;12345678&lt;/Number&gt;
        &lt;Name&gt;Receiver&lt;/Name&gt;
        &lt;/Receiver&gt;
        &lt;Sender&gt;
        &lt;Number&gt;11111&lt;/Number&gt;
        &lt;Name&gt;Sender&lt;/Name&gt;
        &lt;/Sender&gt;
        &lt;Body&gt;Hello World!!!&lt;/Body&gt;
      </Message>
    </u:AddMessage>
  </s:Body>
</s:Envelope>"""

webservice = httplib.HTTP("192.168.1.103:52235")
webservice.putrequest("POST", "/PMR/control/MessageBoxService")
webservice.putheader("Host", "192.168.1.103")
webservice.putheader("User-Agent", "Python post")
webservice.putheader("Content-type", "text/xml; charset=\"UTF-8\"")
webservice.putheader("Content-length", "%d" % len(msg))
webservice.putheader("SOAPAction", "\"urn:samsung.com:service:MessageBoxService:1#AddMessage\"")
webservice.endheaders()
webservice.send(msg)

statuscode, statusmessage, header = webservice.getreply()
print "Response: ", statuscode, statusmessage
print "headers: ", header
res = webservice.getfile().read()
print res
