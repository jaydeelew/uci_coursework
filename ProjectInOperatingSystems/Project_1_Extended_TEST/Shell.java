import java.util.Scanner;

public class Shell {
    public static void main(String[] args) {
        Scanner scan = new Scanner(System.in);
        Mgr mg = new Mgr();

        while (true) {
            System.out.print("> ");
            String[] cmdarr = (scan.nextLine()).split(" ");
            // create
            if (cmdarr[0].compareTo("cr") == 0) {
                if (cmdarr.length < 2) {
                    System.out.println("command must have an argument");
                }
                else {
                    mg.create(Integer.parseInt(cmdarr[1]));
                }
            }
            // destroy
            else if (cmdarr[0].compareTo("de") == 0) {
                if (cmdarr.length < 2) {
                    System.out.println("command must have an argument");
                }
                else {
                    mg.setdestroying(true);
                    int numdestroyed = mg.destroy(Integer.parseInt(cmdarr[1]));
                    mg.setdestroying(false);
                    if (numdestroyed != -1) {
                        System.out.println(numdestroyed + " process(es) destroyed"); 
                    }
                    mg.scheduler();
                }
            }
            // request
            else if (cmdarr[0].compareTo("rq") == 0) {
                if (cmdarr.length < 3) {
                    System.out.println("command must have two arguments");
                }
                else {
                    mg.request(Integer.parseInt(cmdarr[1]), Integer.parseInt(cmdarr[2]));
                }
            }
            // release
            else if (cmdarr[0].compareTo("rl") == 0) {
                if (cmdarr.length < 3) {
                    System.out.println("command must have two arguments");
                }
                else {
                    mg.release(Integer.parseInt(cmdarr[1]), Integer.parseInt(cmdarr[2]));
                }
            }
            // timeout
            else if (cmdarr[0].compareTo("to") == 0) {
                mg.timeout();
            }
            // init
            else if (cmdarr[0].compareTo("in") == 0) {
                mg.init();
            }
            // processlist
            else if (cmdarr[0].compareTo("p") == 0) {
                mg.processlist();
            }
            // resourcelist
            else if (cmdarr[0].compareTo("r") == 0) {
                mg.resourcelist();
            }
            // readylist
            else if (cmdarr[0].compareTo("l") == 0) {
                mg.readylist();
            }
            // tree
            else if (cmdarr[0].compareTo("t") == 0) {
                if (cmdarr.length < 2) {
                    System.out.println("command must have an argument");
                }
                else {
                    System.out.println("tree: " + mg.treemembers(Integer.parseInt(cmdarr[1])));
                }
            }
            // quit
            else if (cmdarr[0].compareTo("q") == 0) {
                scan.close();
                return;
            }
            // invalid command
            else {
                System.out.println("Not a valid command");
            }
        }    
    }
}
