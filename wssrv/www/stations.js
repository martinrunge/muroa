var app = angular.module('stationsApp', ['ngMaterial', 'ngMessages', 'wscmd', 'mclients']);


app.controller('stationsCtrl', ['$scope', '$http', '$mdDialog', 'wscmd', 'mclients', function($scope, $http, $mdDialog, wscmd, mclients) {
    
    $scope.Status = "------";
    $scope.RadioStations = [];
  
    
    
    $http.get("stations.json").
    success(function(data, status, headers, config) {
            $scope.RadioStations = data;
            $scope.Status = "successfully loaded stations list";
    }).
    error(function(data, status, headers, config) {
        $scope.Status = "error loading stations list";
    });


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
    
    $scope.check = function check() {
        return clients.chkbtn();
    }
    
  
    wscmd.reg_cb('muroad',  function (object) {
        $scope.mclients = object;
        console.log(object);
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
    
    wscmd.getCurrentClientList();
    
            
}]);
