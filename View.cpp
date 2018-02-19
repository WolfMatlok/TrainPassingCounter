/* 
 * File:   View.cpp
 * Author: wmk
 * 
 * Created on 19. Februar 2018, 15:05
 */

#include "View.h"

View::View(int argc, char **argv)
: m_application(argc, argv)
{
  m_textEdit.show();
}

View::~View()
{
}

int View::run()
{
  return m_application.exec();
}
