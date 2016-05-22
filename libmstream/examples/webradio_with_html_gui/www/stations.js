var app = angular.module('stationsApp', ['ngMaterial', 'ngMessages', 'wscmd', 'mclients']);


app.controller('stationsCtrl', ['$scope', '$http', 'wscmd', 'mclients', function($scope, $http, wscmd, mclients) {
    
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
    
    wscmd.getCurrentClientList();
    
            
}]);
