import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.StringTokenizer;
import java.io.PrintStream;

public class Driver {
    public static void main(String[] args) throws FileNotFoundException {
        File initfile = null;
        File inputfile = null;
        Scanner initscan = null;
        Scanner inputscan = null;

        try {
            initfile = new File("init-dp.txt");
            initscan = new Scanner(initfile);
        }
        catch (FileNotFoundException e) {
            System.out.println("init-dp.txt not found, place it in same folder as program and try again");
            return;
        }
        System.out.println("init-dp.txt found");

        try {
            inputfile = new File("input-dp.txt");
            inputscan = new Scanner(inputfile);
        }
        catch (FileNotFoundException e) {
            System.out.println("input-dp.txt not found, place it in same folder as program and try again");
            // this is needed if initscan succeeds and inputscan fails
            initscan.close();
            return;
        }
        System.out.println("input-dp.txt found");

        PrintStream systemoutsaved = System.out;
        // handle exception here?
        PrintStream outfile = new PrintStream(new File("output-dp.txt"));
        System.setOut(outfile);

        VMManager vmm = new VMManager();

        // Initializing segmentation table with line 1
        String line1 = initscan.nextLine();
        StringTokenizer st1 = new StringTokenizer(line1);
        vmm.initST(st1); 

        // Initializing page tables with line 2
        String line2 = initscan.nextLine();
        StringTokenizer st2 = new StringTokenizer(line2);
        vmm.initPTs(st2);

        // processing each virtual address
        String line = inputscan.nextLine();
        StringTokenizer st = new StringTokenizer(line);
        while (st.hasMoreTokens()) {
            System.out.print(vmm.translateaddress(st.nextToken()) + " ");
        }

        outfile.close();
        System.setOut(systemoutsaved);
        System.out.println("output-pd.txt written");
        initscan.close();
        inputscan.close();
    }   
}
