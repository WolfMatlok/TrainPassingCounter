/* 
 * File:   View.h
 * Author: wmk
 *
 * Created on 19. Februar 2018, 15:05
 */

#ifndef VIEW_H
#define	VIEW_H

#include <QtGui/qapplication.h>
#include <QtGui/QTextEdit>

class View
{
public:
  View(int argc, char **argv);
  View(const View& orig) = delete;
  virtual ~View();
  
  int run();
private:
  
  QApplication m_application;  
  QTextEdit m_textEdit;
  

};

#endif	/* VIEW_H */

