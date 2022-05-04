var WiFiPortal={SSIDs:[],Networks:[],_msg_proto:function(t){return{$el:document.getElementById(t),show:function(t){this.$el.innerHTML=t,this.$el.style.display="block"},hide:function(){this.$el.style.display="none"}}},init:function(){WiFiPortal.Buttons.init(),WiFiPortal.Info=new WiFiPortal._msg_proto("info"),WiFiPortal.Error=new WiFiPortal._msg_proto("error"),document.getElementById("networks").onchange=function(t){var e=this.value||this.options[this.selectedIndex].value;WiFiPortal.selectNetwork(e)},document.getElementById("networks").onblur=function(t){var e=this.value||this.options[this.selectedIndex].value;WiFiPortal.selectNetwork(e)},WiFiPortal.check(function(t){WiFiPortal.rescan()},!1)},check:function(r,s){WiFiPortal.Buttons.disableAll("Please wait, checking..."),WiFiPortal.rpcCall("GET","Sys.GetInfo",s||!1,!1,function(t){var e="Error";if(t&&!0!==t){var i=t.wifi?t.wifi:t,o=JSON.stringify(i,void 0,2);e=WiFiPortal.highlight(o)}else e="Unable to get info from device";document.getElementById("response").innerHTML=e||"",s||WiFiPortal.Buttons.enableAll(),"function"==typeof r&&r(t)})},Info:{},Error:{},Timers:{clear:function(){WiFiPortal.Timers.Test.remove(),WiFiPortal.Timers.Timeout.remove()},Timeout:{_id:!1,init:function(){WiFiPortal.Timers.Timeout.remove(),WiFiPortal.Timers.Timeout._id=setTimeout(WiFiPortal.Test.timeout,1e3*WiFiPortal.Test._timeout+2e3)},remove:function(){WiFiPortal.Timers.Timeout._id&&(clearTimeout(WiFiPortal.Timers.Timeout._id),WiFiPortal.Timers.Timeout._id=!1)}},Test:{_id:!1,init:function(){WiFiPortal.Timers.Test.remove(),WiFiPortal.Timers.Test._id=setTimeout(WiFiPortal.Test.check,1e3*WiFiPortal.Test._interval)},remove:function(){WiFiPortal.Timers.Test._id&&(clearTimeout(WiFiPortal.Timers.Test._id),WiFiPortal.Timers.Test._id=!1)}}},Buttons:{_proto:function(t,e){var i=document.getElementById(t);return i&&i.addEventListener("click",e),{$el:i,preVal:!1,update:function(t){this.$el.innerHTML=t},disable:function(t){this.preVal||(this.preVal=this.$el.innerHTML),this.update(t),this.$el.disabled=!0},enable:function(){this.preVal&&(this.$el.innerHTML=this.preVal,this.preVal=!1),this.$el.disabled=!1}}},_all:{},_ids:["rescan","save","check"],init:function(){for(var t=0;t<WiFiPortal.Buttons._ids.length;t++){var e=WiFiPortal.Buttons._ids[t];WiFiPortal.Buttons._all[e]=new WiFiPortal.Buttons._proto(e,WiFiPortal[e])}},enableAll:function(){for(var t in WiFiPortal.Buttons._all)WiFiPortal.Buttons._all[t].enable()},disableAll:function(t){for(var e in WiFiPortal.Buttons._all)WiFiPortal.Buttons._all[e].disable(t)}},Test:{_timeout:30,_checks:0,_interval:5,success:!1,timedout:!1,ssid:!1,init:function(){WiFiPortal.Test._checks=0,WiFiPortal.Test.success=!1,WiFiPortal.Test.timedout=!1,setTimeout(WiFiPortal.Test.check,900),WiFiPortal.Timers.Timeout.init()},timeout:function(){if(!WiFiPortal.Test.success){WiFiPortal.Test.timedout=!0,WiFiPortal.Error.show("Test has timed out after "+WiFiPortal.Test._timeout+" seconds. Please check the SSID and Password and try again."),WiFiPortal.Info.hide();var t=document.getElementById("response");t&&(t.innerHTML="")}WiFiPortal.Timers.clear(),WiFiPortal.Buttons.enableAll()},check:function(){WiFiPortal.check(function(t){var e="Error";if(t&&!0!==t)if(t.wifi){var i=JSON.stringify(t.wifi,void 0,2);document.getElementById("response").innerHTML=WiFiPortal.highlight(i),t.wifi.status&&t.wifi.ssid&&("got ip"===t.wifi.status&&t.wifi.ssid===WiFiPortal.Test.ssid?(WiFiPortal.Test.success=!0,WiFiPortal.Error.hide(),WiFiPortal.Info.show("WiFi connection successful! Connected to "+t.wifi.ssid),WiFiPortal.Buttons.enableAll(),WiFiPortal.Timers.clear()):e="WiFi current status is "+t.wifi.status)}else e="Received response, error getting WiFi status";else WiFiPortal.Info.hide(),WiFiPortal.Error.show("Error getting WiFi status, trying again in 5 seconds...");WiFiPortal.Test._checks++,WiFiPortal.Test.success||WiFiPortal.Test.timedout||(WiFiPortal.Error.hide(),WiFiPortal.Info.show(e+", check "+WiFiPortal.Test._checks+", trying again in "+WiFiPortal.Test._interval+" seconds..."),WiFiPortal.Timers.Test.init())},"Checking device WiFi status...")}},save:function(){var t=document.getElementById("networks").value,e=document.getElementById("password").value,i=document.getElementById("puser").value;if(!t||t.length<1)return WiFiPortal.Info.hide(),void WiFiPortal.Error.show("You must select an SSID from the dropdown!");WiFiPortal.Buttons.disableAll("Please wait, sending..."),WiFiPortal.Test.ssid=t,WiFiPortal.rpcCall("POST","WiFi.PortalTest","Sending credentials to device to test...",{ssid:t,pass:e,user:i},function(t){t&&!0!==t&&void 0!==t.result?!1===t.result?(WiFiPortal.Error.show("Error from device setting up STA! Check SSID and Password and try again!"),WiFiPortal.Buttons.enableAll()):(WiFiPortal.Error.hide(),WiFiPortal.Info.show("Device is testing WiFi connection, please wait..."),WiFiPortal.Test.init()):(WiFiPortal.Error.show("Error sending credentials to device, please try again"),WiFiPortal.Buttons.enableAll()),document.body.scrollTop=document.documentElement.scrollTop=0})},rescan:function(){WiFiPortal.Buttons.disableAll("Scanning..."),WiFiPortal.rpcCall("POST","WiFi.PortalScan","Scanning for WiFi networks in range of device...",!1,function(t){if(t&&0<t.length){WiFiPortal.SSIDs=[],WiFiPortal.Networks=[];var o=document.getElementById("networks");o.removeAttribute("disabled"),o.innerHTML='<option value="-1" disabled="disabled" selected="selected">Please select an SSID...</option>',t.forEach(function(t){if(!(-1<WiFiPortal.SSIDs.indexOf(t.ssid))){var e=document.createElement("option"),i=0<parseInt(t.auth)?"🔒 ":" ";e.innerHTML=i+WiFiPortal.rssiToStrength(t.rssi)+"% - "+t.ssid,e.value=t.ssid,o.appendChild(e),WiFiPortal.SSIDs.push(t.ssid),WiFiPortal.Networks.push(t)}}),WiFiPortal.Info.show("Please select from one of the "+WiFiPortal.SSIDs.length+" WiFi networks found.")}else WiFiPortal.Info.hide(),WiFiPortal.Error.show("<sup>*</sup> No networks found, try again...");WiFiPortal.Buttons.enableAll()})},rpcCall:function(t,e,i,o,r){if(httpRequest=new XMLHttpRequest,!httpRequest)return WiFiPortal.Error.show("Unable to create an XMLHttpRequest, try to manually set"),r(!1);void 0!==i&&i&&WiFiPortal.Info.show(i),httpRequest.onreadystatechange=function(){if(httpRequest.readyState!==XMLHttpRequest.DONE)return!1;if(200===httpRequest.status){var t=JSON.parse(httpRequest.responseText);r(t)}else httpRequest.responseText&&0<httpRequest.responseText.length?(WiFiPortal.Error.show("Error from device ( "+httpRequest.responseText+" ) -- Please try again"),r(!0)):r(!1)},httpRequest.open(t,"/rpc/"+e),httpRequest.setRequestHeader("Content-Type","application/json"),httpRequest.send(JSON.stringify(o))},rssiToStrength:function(t){return quality=0===t||t<=-100?0:-50<=t?100:2*(t+100),quality},highlight:function(t){return(t=t.replace(/&/g,"&amp;").replace(/</g,"&lt;").replace(/>/g,"&gt;")).replace(/("(\\u[a-zA-Z0-9]{4}|\\[^u]|[^\\"])*"(\s*:)?|\b(true|false|null)\b|-?\d+(?:\.\d*)?(?:[eE][+\-]?\d+)?)/g,function(t){var e="number";return/^"/.test(t)?e=/:$/.test(t)?"key":"string":/true|false/.test(t)?e="boolean":/null/.test(t)&&(e="null"),'<span class="'+e+'">'+t+"</span>"})},selectNetwork:function(i){var o=document.getElementById("passwrap"),r=document.getElementById("peapuserwrap"),s=(document.getElementById("networks"),!(r.style.display="none"));WiFiPortal.Networks.forEach(function(t){if(t.ssid===i){var e=parseInt(t.auth);return 0===e?o.style.display="none":5===e?(o.style.display="block",r.style.display="block"):o.style.display="block",!(s=!0)}}),s||(o.style.display="block")}};document.addEventListener("DOMContentLoaded",WiFiPortal.init);