'use strict';
var mclients = angular.module('mclients', ['ngWebsocket']);

mclients.factory('mclients', ['$websocket', function($websocket) {
    var clist = [];
    var url = location.origin;
    
//    var ws = $websocket.$new('ws://' + location.host );                  
    // var ws = $websocket.$new('ws://localhost:8888');
    
                  
//    var chkbtn = function checkbtn() {
//        var ws = $websocket.$get('ws://localhost:8888')
//                .$emit('my msg type', 'my data for now');
//    }
      
    return {
//        ws: ws,
        clist: clist,
//        chkbtn: chkbtn
    };
}])

mclients.run(['mclients', function (mclients) {
//        var ws = mclients.ws;
//        ws.$on('$open', function () {
//            console.log('Oh my gosh, websocket is really open! Fukken awesome!  clist: ' + mclients.clist);
//
//            var data = {
//                level: 1,
//                text: 'ngWebsocket rocks!',
//                array: ['one', 'two', 'three'],
//                nested: {
//                    level: 2,
//                    deeper: [{
//                        hell: 'yeah'
//                    }, {
//                        so: 'good'
//                    }]
//                }
//            };
//
//            ws.$emit('ping', 'hi listening websocket server') // send a message to the websocket server
//              .$emit('pong', data);
//          })
//          ws.$on('pong', function (data) {
//            console.log('Pong: The websocket server has sent the following data:');
//            console.log(data);
//
//            ws.$close();
//          })
//          ws.$on('$close', function () {
//            console.log('Noooooooooou, I want to have more fun with ngWebsocket, damn it!');
//          })
//          ws.$on('$message', function (message) {
//                var c = eval(message);
//                $rootScope.$apply(function( ) {
//                    console.log(clients.clients);
//                    clients.clients = c;
//                    $rootScope.mclients = clients.clients;
//                });
//          });
          
    
}])


