/*
  Default KWin client

  Copyright 2000
    Rik Hemsley <rik@kde.org>

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; see the file COPYING.  If not, write to
  the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
  Boston, MA 02111-1307, USA.
*/

#ifndef DEFAULT_TITLE_BAR_H
#define DEFAULT_TITLE_BAR_H

#include <qwidget.h>

namespace Default
{

class Manager;
class QuestionButton;
class StickyButton;
class CloseButton;
class TitleText;
class IconifyButton;
class MaximiseButton;

class TitleBar : public QWidget
{
  public:

    TitleBar(Manager * client);
    virtual ~TitleBar();

    void updateDisplay();
    void updateText();
    void updateMaximise(bool);

    void setActive(bool);

  protected:

    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);

    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

  private:

    CloseButton * close_;

    TitleText * text_;

    QuestionButton * question_;
    StickyButton * sticky_;
    IconifyButton * iconify_;
    MaximiseButton * maximise_;
};

} // End namespace

#endif

// vim:ts=2:sw=2:tw=78
