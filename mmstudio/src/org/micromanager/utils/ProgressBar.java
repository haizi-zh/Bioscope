///////////////////////////////////////////////////////////////////////////////
//FILE:          ProgressBarjava
//PROJECT:       Micro-Manager
//SUBSYSTEM:     mmstudio
//-----------------------------------------------------------------------------
//
// AUTHOR:       Nico Stuurman, nico@cmp.ucsf.edu, December, 2006
//
// COPYRIGHT:    University of California, San Francisco, 2006
//
// LICENSE:      This file is distributed under the BSD license.
//               License text is included with the source distribution.
//
//               This file is distributed in the hope that it will be useful,
//               but WITHOUT ANY WARRANTY; without even the implied warranty
//               of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
//               IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//               CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//               INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES.
//
// CVS:          $Id$
//
package org.micromanager.utils;

import java.awt.BorderLayout;

import javax.swing.BorderFactory;
import javax.swing.JComponent;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JProgressBar;
import javax.swing.SwingUtilities;


public class ProgressBar extends JPanel {
   private static final long serialVersionUID = 1L;
   private JProgressBar progressBar;
   private JFrame frame;

   public ProgressBar (String windowName, int start, int end) {
      super(new BorderLayout());
      
      frame = new JFrame(windowName);
      frame.setDefaultCloseOperation (JFrame.DISPOSE_ON_CLOSE);

      progressBar = new JProgressBar(start,end);
      progressBar.setValue(0);
      JPanel panel = new JPanel();
      panel.add(progressBar);
      add(panel, BorderLayout.PAGE_START);
      panel.setBorder(BorderFactory.createEmptyBorder(20, 20, 20, 20));

      JComponent newContentPane = panel;
      newContentPane.setOpaque(true);
      frame.setContentPane(newContentPane);

      frame.pack();
      frame.setBounds(0,0,250,100);
      frame.setLocationRelativeTo(null);
      frame.setVisible(true);
   }

   public void setProgress(int progress) {
      progressBar.setValue(progress);
      progressBar.repaint();
   }

   public void setVisible(boolean visible) {
      frame.setVisible(visible);
   }

    public void setRange(int min, int max) {
        progressBar.setMinimum(min);
        progressBar.setMaximum(max);
    }

   /*
   public static void main(String[] args) {
      ProgressBar testBar = new ProgressBar ("Opening File...", 0, 100);
      for (int i=0; i<=100; i++) {
         try {
            Thread.sleep(10);
         } catch (InterruptedException ignore) { ReportingUtils.logError(ignore); }
         testBar.setProgress(i);
      }
      testBar.setVisible(false);
   }
   */
}
