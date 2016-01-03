var app = angular.module('stationsApp', []);


app.controller('stationsCtrl', ['$scope', '$http', function($scope, $http) {
    
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


    $scope.stationChanged = function($index) {
//        $scope.Status = "station changed to " + $scope.RadioStations[$index].Name;
        $http.get("REST/station.json?station=" + $scope.RadioStations[$index].ID).
            success(function(data, status, headers, config) {
                $scope.Status = "switched station to " + $scope.RadioStations[$index].Name;
                    }).
            error(function(data, status, headers, config) {
                    $scope.Status = "error switching station";
            });
    };
    
    $scope.Stop = function() {
        $http.get("REST/stop.json").
            success(function(data, status, headers, config) {
                $scope.Status = "stopping playback ...";
                    }).
            error(function(data, status, headers, config) {
                    $scope.Status = "error stopping playback ...";
            });
    };
            
            
}]);
