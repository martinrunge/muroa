/*
 * MediaprocessingTestApp.cpp
 *
 *  Created on: 11 Jul 2010
 *      Author: martin
 */

#include <QTextCodec>

#include <QApplication>
#include <qxcppunit/testrunner.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>


int main(int argc, char* argv[])
{

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    QApplication app(argc, argv);

    // Get the top level suite from the registry
    CppUnit::Test *suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();

    // Adds the test to the list of test to run
    //CppUnit::TextUi::TestRunner runner;
    QxCppUnit::TestRunner runner;
    //CppUnit::QtUi::TestRunner runner;
    runner.addTest( suite );
    //runner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());

    // Change the default outputter to a compiler error format outputter
    // runner.setOutputter( new CppUnit::CompilerOutputter( &runner.result(), std::cerr ) );

    // Run the tests.
    // bool wasSucessful = runner.run();    // CppUnit::TextUi::TestRunner returns bool here.
    runner.run();
    bool wasSucessful = true;

    // Return error code 1 if the one of test failed.
    return wasSucessful ? 0 : 1;
}
