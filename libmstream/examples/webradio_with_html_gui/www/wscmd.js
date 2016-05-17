'use strict';
var wscmd = angular.module('wscmd', ['ngWebsocket']);

wscmd.factory('wscmd', ['$websocket', function($websocket) {

    var url = location.origin.host;
    
    var ws = $websocket.$new('ws://' + location.host );                  
    var callbacks = {};

    var reg_cb = function reg_cb(event, callback) {
                              if(typeof callback === "function") {
                                  callbacks[event] = callback;
                              }
                              else {
                                  console.error("callback is not a function!");
                              }
                        }
                 
    var unreg_cb = function unreg_cb(event) {
                                delete callbacks[event];
                            }
    
    var playctrl = function playctrl(action) {
       var playctrldata = {};
       playctrldata["action"] = action;
       
       console.log("emit: " + playctrldata);
       ws.$emit('playctrl', playctrldata);
    }
    
    var start = function start() {
        playctrl("start");
    }
    
    var stop = function stop() {
        playctrl("stop");
    }
    
    var pause = function pause() {
        playctrl("pause");
    }
    
    var changeStation = function changeStation(stationID) {

        var chStationData = {};
        chStationData["stationID"] = stationID;
       
       ws.$emit('changeStation',chStationData);
    }

    
    return {
        ws: ws,
        callbacks: callbacks,
        reg_cb: reg_cb,
        unreg_cb: unreg_cb,
        start: start,
        stop: stop,
        pause: pause,
        changeStation: changeStation,
    };
}])

wscmd.run(['wscmd', function (wscmd) {
        var ws = wscmd.ws;
        var callbacks = wscmd.callbacks;
        
        ws.$on('$open', function () {
            console.log('Oh my gosh, websocket is really open! Fukken awesome!  clist: ' + wscmd.clist);

            var data = {
                level: 1,
                text: 'ngWebsocket rocks!',
                array: ['one', 'two', 'three'],
                nested: {
                    level: 2,
                    deeper: [{
                        hell: 'yeah'
                    }, {
                        so: 'good'
                    }]
                }
            };

            ws.$emit('ping', 'hi listening websocket server') // send a message to the websocket server
              .$emit('pong', data);
          })
          ws.$on('pong', function (data) {
            console.log('Pong: The websocket server has sent the following data:');
            console.log(data);

            ws.$close();
          })
          ws.$on('$close', function () {
            console.log('Noooooooooou, I want to have more fun with ngWebsocket, damn it!');
          })
          
          
          ws.$on('$message', function (message) {
              // get type from message
              console.log(message);
              var object = angular.fromJson(message);
              var event = object.event;
              if(event in callbacks) {
                // call callback of type with data
                var data = object.data;
                callbacks[event](data);
              }
              else {
                  console.log("no callback registered for type '" + type + "'");
              }
          })
    
}])


