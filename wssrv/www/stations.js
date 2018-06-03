var app = angular.module('stationsApp', ['ngMaterial', 'ngAnimate', 'ngAria', 'ngMessages', 'wscmd', 'mclients']);


app.controller('stationsCtrl', ['$scope', '$http', '$mdDialog', 'wscmd', 'mclients', function($scope, $http, $mdDialog, wscmd, mclients) {
    
    $scope.Status = "------";
    $scope.RadioStations = [];
  
    
    
    $http.get("stations.json").then( successCB, errorCB);
    
    function successCB(response, status, headers, config) {
            $scope.RadioStations = response.data;
            $scope.Status = "successfully loaded stations list";
    };
    
    function errorCB(response, status, headers, config) {
        $scope.Status = "error loading stations list";
    };


//    $scope.stationChanged = wscmd.changeStation;
    $scope.stationChanged = function($index) {
        $scope.Status = "station changed to " + $scope.RadioStations[$index].Name;
        wscmd.changeStation($scope.RadioStations[$index].ID);
    }
    
//    function($index) {
//        $scope.Status = "station changed to " + $scope.RadioStations[$index].Name;
//        $http.get("REST/station.json?station=" + $scope.RadioStations[$index].ID).
//            success(function(data, status, headers, config) {
//                $scope.Status = "switched station to " + $scope.RadioStations[$index].Name;
//                    }).
//            error(function(data, status, headers, config) {
//                    $scope.Status = "error switching station";
//            });
//    };
    
    $scope.Stop = wscmd.stop;
    $scope.mclients = mclients.clist;
    $scope.activateClient = wscmd.activateClient;
    
    $scope.adjVol = function(event, mclient, delta) { 
        mclient.volume += delta;
    };
    
    $scope.openPropDlg = function(mclient, event) {
        $mdDialog.show({
            controller: DialogController,
            scope: $scope,
            preserveScope: true,
            templateUrl: 'ClientPropertyDialog.html',
            parent: angular.element(document.body),
            targetEvent: event,
            locals: {
               mclient: mclient,
            },
            clickOutsideToClose:true,
            fullscreen: $scope.customFullscreen // Only for -xs, -sm breakpoints.
        })
        .then(function(answer) {
            $scope.status = 'You said the information was "' + answer + '".';
        }, function() {
            $scope.status = 'You cancelled the dialog.';
        });
        
        function DialogController($scope, $mdDialog, mclient) {
            $scope.mclient = mclient;
            $scope.hide = function() {
                $mdDialog.hide();
            };

            $scope.cancel = function() {
                $mdDialog.cancel();
            };

            $scope.answer = function(answer) {
                $mdDialog.hide(answer);
            };
        };

    };
        
//    $scope.openPropDlg = function(mclient, event) {
//      $mdDialog.show(
//        $mdDialog.alert()
//          .title('mclient.name')
//          .textContent(mclient.session)
//          .ariaLabel('Person inspect demo')
//          .ok('Neat!')
//          //.targetEvent(event)
//      );
//    };

    
    
    $scope.check = function check() {
        return clients.chkbtn();
    }
    
  
    wscmd.reg_cb('muroad',  function (object) {
        
        $scope.mclients.forEach (function(element, index, array) {
            if(element.hasOwnProperty("unregwatch")) {
                element.unregwatch();
            }
        });

        console.log(object);
        $scope.mclients = object;

        $scope.mclients.forEach(function(element, index, array) {
            console.log("Index " + index + " : " + element.name);
 
            // place different icons for session members and non session members
            if(element.member == true) {
                element.icon = "icons/ic_speaker_black_24px.svg";
            } else {
                element.icon = "icons/ic_close_24px.svg";
            }
            element.unregwatch = $scope.$watch(
                // This function returns the value being watched. It is called for each turn of the $digest loop
                function() { return element.volume; },
                // This is the change listener, called when the value returned from the above function changes
                function(newValue, oldValue) {
                    if ( newValue !== oldValue ) {
                    // Only if volume changed
                    wscmd.adjVol(element.name, newValue);
                }
            });
            element.settingsicon = "icons/ic_more_horiz_24px.svg";
            if(('lasterrmsg' in element) && element.lasterrmsg) {
                 element.settingsicon = "icons/round-warning-24px.svg";
            }
            
        });
        
        $scope.$apply();
    });
        
    wscmd.reg_cb('inform',  function (object) {
      // Appending dialog to document.body to cover sidenav in docs app
      // Modal dialogs should fully cover application
      // to prevent interaction outside of dialog
      $mdDialog.show(
        $mdDialog.alert()
          .parent(angular.element(document.querySelector('#popupContainer')))
          .clickOutsideToClose(true)
          .title(object.caption)
          .textContent(object.message)
          .ariaLabel('Alert Dialog Demo')
          .ok('Got it!')
          .targetEvent(object)
      );
      console.log(object);
      $scope.$apply();
    });
    
    wscmd.reg_cb('session_error',  function (object) {
        $scope.mclients.forEach( function(element, index, array) {
            console.log("Index " + index + " : " + element.name);
        });
      $scope.$apply();
    });


    wscmd.getCurrentClientList();
    
            
}]);
