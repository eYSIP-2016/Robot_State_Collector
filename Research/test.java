
import com.sun.org.apache.xml.internal.security.utils.Base64;
import gnu.io.*;
import java.io.IOException;
import java.io.InputStream;
import java.io.*;
import java.util.Enumeration;
import java.util.TooManyListenersException;
import javax.crypto.Cipher;
import javax.crypto.KeyGenerator;
import javax.crypto.SecretKey;
import java.security.KeyStore;


/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/**
 *
 * @author Amanpreet Singh
 */
public class test extends javax.swing.JFrame implements Runnable, SerialPortEventListener {

    /**
     * Creates new form test
     */
    static CommPortIdentifier portId;
    static Enumeration portList;
    InputStream inputStream;
    SerialPort serialPort;
    Thread readThread;
    String ans = "";
    KeyGenerator keygenerator;
    SecretKey myDesKey;
    java.io.FileInputStream fis = null;
    java.io.FileOutputStream fos = null;
    KeyStore ks;

    public test() {
        initComponents();
    }

    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jButton1 = new javax.swing.JButton();
        jComboBox1 = new javax.swing.JComboBox<>();
        jButton2 = new javax.swing.JButton();
        jScrollPane1 = new javax.swing.JScrollPane();
        jTextArea1 = new javax.swing.JTextArea();
        jLabel1 = new javax.swing.JLabel();
        jButton3 = new javax.swing.JButton();
        jButton4 = new javax.swing.JButton();
        jButton5 = new javax.swing.JButton();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);

        jButton1.setText("Search for available ports");
        jButton1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton1ActionPerformed(evt);
            }
        });

        jComboBox1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jComboBox1ActionPerformed(evt);
            }
        });

        jButton2.setText("DISCONNECT");
        jButton2.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton2ActionPerformed(evt);
            }
        });

        jTextArea1.setColumns(20);
        jTextArea1.setRows(5);
        jScrollPane1.setViewportView(jTextArea1);

        jLabel1.setText("Not Connected");

        jButton3.setText("Clear");
        jButton3.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton3ActionPerformed(evt);
            }
        });

        jButton4.setText("End of run");
        jButton4.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton4ActionPerformed(evt);
            }
        });

        jButton5.setText("Decrypt");
        jButton5.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton5ActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(32, 32, 32)
                        .addComponent(jButton1, javax.swing.GroupLayout.PREFERRED_SIZE, 200, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(72, 72, 72)
                        .addComponent(jButton3, javax.swing.GroupLayout.PREFERRED_SIZE, 103, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addGap(79, 79, 79)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jComboBox1, javax.swing.GroupLayout.PREFERRED_SIZE, 69, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLabel1, javax.swing.GroupLayout.PREFERRED_SIZE, 88, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 58, Short.MAX_VALUE)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jButton2)
                    .addComponent(jButton4)
                    .addComponent(jButton5))
                .addGap(23, 23, 23))
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jScrollPane1)
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGap(22, 22, 22)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(jButton1, javax.swing.GroupLayout.PREFERRED_SIZE, 33, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(jLabel1, javax.swing.GroupLayout.PREFERRED_SIZE, 30, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(jButton3, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                        .addGap(28, 28, 28))
                    .addGroup(layout.createSequentialGroup()
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(jComboBox1, javax.swing.GroupLayout.PREFERRED_SIZE, 23, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(jButton2))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jButton4)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(jButton5)
                        .addGap(18, 18, 18)))
                .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 167, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(31, 31, 31))
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void jComboBox1ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jComboBox1ActionPerformed
        // TODO add your handling code here:
        Object selectedItem = jComboBox1.getSelectedItem();

        String com = selectedItem.toString();

        SimpleRead(com);
    }//GEN-LAST:event_jComboBox1ActionPerformed

    private void jButton1ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton1ActionPerformed
        // TODO add your handling code here:
        ans = "";
        java.util.Enumeration<CommPortIdentifier> portEnum = CommPortIdentifier.getPortIdentifiers();
        int i = 0;
        String[] r = new String[5];

        while (portEnum.hasMoreElements() && i < 5) {
            CommPortIdentifier portIdentifier = portEnum.nextElement();

            r[i] = portIdentifier.getName();//+  " - " +  getPortTypeName(portIdentifier.getPortType()) ;

            i++;

        }

        jComboBox1.setModel(new javax.swing.DefaultComboBoxModel(r));


    }//GEN-LAST:event_jButton1ActionPerformed

    private void jButton2ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton2ActionPerformed
        // TODO add your handling code here:
        serialPort.removeEventListener();
        serialPort.close();
        jLabel1.setText("Disconnected");
        jTextArea1.setText(ans);
    }//GEN-LAST:event_jButton2ActionPerformed

    private void jButton3ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton3ActionPerformed
        // TODO add your handling code here:
        jTextArea1.setText("");
    }//GEN-LAST:event_jButton3ActionPerformed

    private void jButton4ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton4ActionPerformed
        // TODO add your handling code here:
        String s = "";
        BufferedWriter writer = null;
        try {
            keygenerator = KeyGenerator.getInstance("AES");
            ks = KeyStore.getInstance("JCEKS");
            myDesKey = keygenerator.generateKey();

            String password = "EYSIP2016";
            char[] xy = password.toCharArray();
            try {
                fis = new java.io.FileInputStream("keyStoreName.jceks");
                ks.load(fis, xy);
            } finally {
                if (fis != null) {
                    fis.close();
                }
            }
            KeyStore.ProtectionParameter protParam = new KeyStore.PasswordProtection(xy);
            KeyStore.SecretKeyEntry skEntry = new KeyStore.SecretKeyEntry(myDesKey);
            ks.setEntry("secretKeyAlias", skEntry, protParam);
            try {
                fos = new java.io.FileOutputStream("keyStoreName.jceks");
                ks.store(fos, xy);
                String stringKey = myDesKey.toString();
                System.out.println("Stored Key: " + stringKey);
            } finally {
                if (fos != null) {
                    fos.close();
                }
            }

            Cipher desCipher;
            desCipher = Cipher.getInstance("AES/ECB/PKCS5Padding");

            byte[] ansen = ans.getBytes("UTF8");

            desCipher.init(Cipher.ENCRYPT_MODE, myDesKey);
            byte[] textEncrypted = desCipher.doFinal(ansen);

            s = new String(textEncrypted);
        } catch (Exception e) {
            System.out.println("Exception" + e.toString());
        }

        try {
            writer = new BufferedWriter(new FileWriter("output.txt"));
            writer.write(s);

        } catch (IOException e) {
        } finally {
            try {
                if (writer != null) {
                    writer.close();
                }
            } catch (IOException e) {
            }
        }

    }//GEN-LAST:event_jButton4ActionPerformed

    private void jButton5ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton5ActionPerformed
        // TODO add your handling code here:
        String password = "EYSIP2016";
        BufferedWriter writer = null;
        char[] xy = password.toCharArray();
        KeyStore.ProtectionParameter protParam = new KeyStore.PasswordProtection(xy);
        try {

            try {
                fis = new java.io.FileInputStream("keyStoreName.jceks");
                ks.load(fis, xy);
            } finally {
                if (fis != null) {
                    fis.close();
                }

            }
            KeyStore.SecretKeyEntry skEntry = (KeyStore.SecretKeyEntry) ks.getEntry("secretKeyAlias", protParam);
            myDesKey = skEntry.getSecretKey();
            String stringKey = myDesKey.toString();
            System.out.println("Stored Key: " + stringKey);

            Cipher desCipher;
            desCipher = Cipher.getInstance("AES/ECB/PKCS5Padding");

            BufferedReader br = new BufferedReader(new FileReader("output.txt"));
            try {
                StringBuilder sb = new StringBuilder();
                String line = br.readLine();

                while (line != null) {
                    sb.append(line);
                    sb.append(System.lineSeparator());
                    line = br.readLine();
                }
                ans = sb.toString();
            desCipher.init(Cipher.DECRYPT_MODE, myDesKey);
            byte[] ansen = ans.getBytes("UTF8");
            byte[] textDecrypted = desCipher.doFinal(ansen);
            String s = new String(textDecrypted);
            
            try {
            writer = new BufferedWriter(new FileWriter("output.txt"));
            writer.write(s);

        } catch (IOException e) {
        } finally {
            try {
                if (writer != null) {
                    writer.close();
                }
            } catch (IOException e) {
            }
        }
            
            
            } finally {
                br.close();
            }

        } catch (Exception e) {
            System.out.println("Exception" + e.toString());
        }
    }//GEN-LAST:event_jButton5ActionPerformed

    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {
        /* Set the Nimbus look and feel */
        //<editor-fold defaultstate="collapsed" desc=" Look and feel setting code (optional) ">
        /* If Nimbus (introduced in Java SE 6) is not available, stay with the default look and feel.
         * For details see http://download.oracle.com/javase/tutorial/uiswing/lookandfeel/plaf.html
         */
        try {
            for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
                if ("Nimbus".equals(info.getName())) {
                    javax.swing.UIManager.setLookAndFeel(info.getClassName());
                    break;
                }
            }
        } catch (ClassNotFoundException ex) {
            java.util.logging.Logger.getLogger(test.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(test.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(test.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(test.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>

        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new test().setVisible(true);
            }
        });
    }

    public void SimpleRead(String com) {
        portList = CommPortIdentifier.getPortIdentifiers();

        while (portList.hasMoreElements()) {
            portId = (CommPortIdentifier) portList.nextElement();
            if (portId.getPortType() == CommPortIdentifier.PORT_SERIAL) {
                if (portId.getName().equals(com)) {
                    //                if (portId.getName().equals("/dev/term/a")) {
                    try {
                        serialPort = (SerialPort) portId.open("SimpleReadApp", 2000);
                        jLabel1.setText("Connected");
                    } catch (PortInUseException e) {
                        System.out.println(e);
                    }
                    try {
                        inputStream = serialPort.getInputStream();
                    } catch (IOException e) {
                        System.out.println(e);
                    }
                    try {
                        serialPort.addEventListener(this);
                    } catch (TooManyListenersException e) {
                        System.out.println(e);
                    }
                    serialPort.notifyOnDataAvailable(true);
                    try {
                        serialPort.setSerialPortParams(9600,
                                SerialPort.DATABITS_8,
                                SerialPort.STOPBITS_1,
                                SerialPort.PARITY_NONE);
                    } catch (UnsupportedCommOperationException e) {
                        System.out.println(e);
                    }
                    readThread = new Thread(this);
                    readThread.start();
                }
            }
        }

    }

    @Override
    public void run() {

        while (true) {
            byte[] readBuffer = new byte[1];

            try {
                if (inputStream.available() > 0) {
                    //int numBytes = inputStream.read(readBuffer);
                    inputStream.read(readBuffer);
                    String y = new String(readBuffer);
                    System.out.print(new String(readBuffer));
                    jTextArea1.append(y);
                    ans = ans.concat(y);
                }
            } catch (IOException e) {
                System.out.println(e);
            }
        }
    }

    public void serialEvent(SerialPortEvent event) {
        switch (event.getEventType()) {
            case SerialPortEvent.BI:
            case SerialPortEvent.OE:
            case SerialPortEvent.FE:
            case SerialPortEvent.PE:
            case SerialPortEvent.CD:
            case SerialPortEvent.CTS:
            case SerialPortEvent.DSR:
            case SerialPortEvent.RI:
            case SerialPortEvent.OUTPUT_BUFFER_EMPTY:
                break;
            case SerialPortEvent.DATA_AVAILABLE:
                /*byte[] readBuffer = new byte[1];

                try {
                    while (inputStream.available() > 0) {
                        //int numBytes = inputStream.read(readBuffer);
                        inputStream.read(readBuffer);
                    }

                    String y = new String(readBuffer);
                    System.out.print(new String(readBuffer));
                    jTextArea1.append(y);
                } catch (IOException e) {
                    System.out.println(e);
                }*/
                break;
        }
    }


    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton jButton1;
    private javax.swing.JButton jButton2;
    private javax.swing.JButton jButton3;
    private javax.swing.JButton jButton4;
    private javax.swing.JButton jButton5;
    private javax.swing.JComboBox<String> jComboBox1;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JTextArea jTextArea1;
    // End of variables declaration//GEN-END:variables
}
