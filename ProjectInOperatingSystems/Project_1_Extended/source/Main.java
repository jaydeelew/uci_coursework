import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.StringTokenizer;
import java.io.PrintStream;

public class Main {
    public static void main(String[] args) throws FileNotFoundException {
        File infile = null;
        Scanner filescan = null;
        try {
            infile = new File("input.txt");
            filescan = new Scanner(infile);
        }
        catch (FileNotFoundException e) {
            System.out.println("input.txt not found, place it in same folder as program and try again");
            return;
        }
        System.out.println("input.txt found");
        PrintStream systemoutsaved = System.out;
        PrintStream outfile = new PrintStream(new File("output.txt")); // handle exception here?
        System.setOut(outfile);

        Mgr mg = new Mgr();
        while (filescan.hasNextLine()) {
            String line = filescan.nextLine();
            if (line.equals("")) {
                System.out.println("");
                continue;
            }
            StringTokenizer st = new StringTokenizer(line);
            String command = st.nextToken();

            // create
            if (command.equals("cr")) {
                String priority = st.nextToken();
                System.out.print(mg.create(Integer.parseInt(priority)) + " ");
            }
            // destroy
            else if (command.equals("de")) {
                String process = st.nextToken();
                mg.setdestroying(true);
                int numdestroyed = mg.destroy(Integer.parseInt(process));
                mg.setdestroying(false);
                if (numdestroyed == -1) {
                    System.out.print(numdestroyed + " "); 
                }
                else {
                    System.out.print(mg.scheduler() + " ");
                }
            }
            // request
            else if (command.equals("rq")) {
                String resource = st.nextToken();
                String amount = st.nextToken();
                System.out.print(mg.request(Integer.parseInt(resource), Integer.parseInt(amount)) + " ");
            }
            // release
            else if (command.equals("rl")) {
                String resource = st.nextToken();
                String amount = st.nextToken();
                System.out.print(mg.release(Integer.parseInt(resource), Integer.parseInt(amount)) + " ");
            }   
            // timeout
            else if (command.equals("to")) {
                System.out.print(mg.timeout() + " ");
            }
            // init
            else if (command.equals("in")) {
                System.out.print(mg.init() + " ");
            }
        }
        outfile.close();
        System.out.println("");
        System.setOut(systemoutsaved);
        System.out.println("output.txt written");
        filescan.close();
    }
}
