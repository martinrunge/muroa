var app = angular.module('muroa', ['ngMaterial', 'ngMessages', 'wscmd']);

// 'material.svgAssetsCache',

app.controller('MainCtrl', ['$scope', 'stations', 'wscmd', function($scope, stations, wscmd) {
    $scope.name = 'World';
    $scope.stations = stations.slist;
    
    $scope.stationChanged = stations.stationChanged;
    $scope.Stop = stations.stop();
    
    $scope.wscmd = wscmd.clist;
    $scope.check = function check() {
        return clients.chkbtn();
    }
  
    wscmd.ws.$on('$message', function (message) {
        var c = eval(message);
                
        $scope.wscmd = c;
        console.log(c);
        $scope.$apply();
    });

}]);

