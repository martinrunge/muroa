/*
 * MediaprocessingTestApp.cpp
 *
 *  Created on: 11 Jul 2010
 *      Author: martin
 */

#include <QTextCodec>

#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/ui/qt/TestRunner.h>


int main(int argc, char* argv[])
{

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    // Get the top level suite from the registry
    CppUnit::Test *suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();

    // Adds the test to the list of test to run
    CppUnit::TextUi::TestRunner runner;
    //CppUnit::QtUi::TestRunner runner;
    runner.addTest( suite );

    // Change the default outputter to a compiler error format outputter
    runner.setOutputter( new CppUnit::CompilerOutputter( &runner.result(),
                                                       std::cerr ) );

    // Run the tests.
    bool wasSucessful = runner.run();
    /* bool wasSucessful = true;
    runner.run(true);*/

    // Return error code 1 if the one of test failed.
    return wasSucessful ? 0 : 1;
}
