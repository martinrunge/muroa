var app = angular.module('stationsApp', []);


app.controller('stationsCtrl', ['$scope', '$http', function($scope, $http) {
    
    $scope.Message = "leere Message";
    $scope.RadioStations = [];
  
    
    
    $http.get("stations.json").
    success(function(data, status, headers, config) {
            $scope.RadioStations = data;
            $scope.Status = "successfully loaded stations list";
    }).
    error(function(data, status, headers, config) {
        $scope.Message = "error loading stations list";
    });


    $scope.stationChanged = function($index) {
//        $scope.Status = "station changed to " + $scope.RadioStations[$index].Name;
        $http.get("REST/station.json?station=" + $scope.RadioStations[$index].ID).
            success(function(data, status, headers, config) {
                $scope.Status = "switched station to " + $scope.RadioStations[$index].Name;
                    }).
            error(function(data, status, headers, config) {
                    $scope.Message = "error switching station";
            });

    };
    
    $scope.ButtonClick = function() {
        $scope.Message = "ButtonClicked";
        init();
    };
            
            
}]);
