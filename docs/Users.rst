Users
=====

Installation
------------


Install playback clients on RaspberryPi with raspbian stretch
-------------------------------------------------------------

Binary packages of the render client are available for raspbian stretch. To install them on a Raspberry Pi:


.. code-block:: bash

    # create an etry in sources.list
    sudo bash -c 'cat << EOF > /etc/apt/sources.list.d/muroa.list
    deb [arch=armhf] http://deb.muroa.org/raspbian stretch main
    EOF'
    
    # add the signing key to your keyring
    sudo bash -c 'wget -O - https:https://muroa.org/sites/default/files/martinrunge.asc | apt-key add -'
    
    sudo apt-get update
    sudo apt-get install muroad


Build from source
-----------------

Except for RaspberryPi, there are no binary packages available by now. I plan to offer binary packages for the major Linux distributions in the future. Until they are available, you have to build muroa from source:


Build from source
Prerequisites:

* "build-essential"   (so called on Ubuntu, includes tools like make, gcc, automake)
* git
* cmake
* libboost-system-dev
* libbost-filesystem-dev
* libboost-date-time-dev
* libboost-program-options-dev


Ubuntu and its flavours: 

.. code-block:: bash

    sudo apt-get install build-essential
    sudo apt-get install git
    sudo apt-get install cmake
    
    # cmake will ask you to install all of the following packages:
    
    sudo apt-get install libboost-system-dev \
                     libboost-filesystem-dev \
                     libboost-date-time-dev \
                     libboost-program-options-dev \
                     libboost-regex-dev \
                     libqt4-dev \
                     libasound2-dev \
                     libao-dev \
                     liblog4cplus-dev \
                     libavahi-client-dev \
                     libavahi-qt4-dev \
                     libsamplerate0-dev \
                     zlib1g-dev \
                     libavcodec-dev \
                     libavformat-dev \
                     libexpat1-dev \
                     libtag1-dev \
                     libsqlite3-dev\
                     
                     
Get the source from github 
--------------------------

.. code-block:: bash

    mkdir -p ~/git-repo
    cd ~/git-repo
    
    git clone https://github.com/martinrunge/muroa.git
    
    mkdir -p build-x86
    cd build-x86
    
    cmake ../muroa   
    

