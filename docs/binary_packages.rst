Binary Packages
===============


RaspberryPi (raspbian stretch)
------------------------------

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
