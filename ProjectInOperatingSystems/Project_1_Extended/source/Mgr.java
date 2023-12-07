import java.util.LinkedList;
import java.util.ListIterator;

public class Mgr {
    private static final int PCBSIZE = 16;
    private static final int RCBSIZE = 4;
    private boolean destroying = false;
    private PCB[] pcbarr;
    private RCB[] rcbarr;
    private RL readylist;

    public Mgr() {
        init();
    }

    public void setdestroying(boolean set) {
        destroying = set;
    }

    private int pcbopenslot() {
        int slot = 0; // slot 0 always occupied by process 0
        for (; slot < PCBSIZE; slot++) {
            if (pcbarr[slot] == null) {
                break;
            }
        }
        return slot > 0 ? slot : -1;
    }

    private boolean processexists(int pid) {
            if (pcbarr[pid] != null) {
                return true;
            }
            return false;
    }

    private int processpriority(int pid) {
        return pcbarr[pid].priority;
    }

    private void remove(int pid) {
        // remove process from parent's list
        int ppid = pcbarr[pid].parent;
        if (ppid != -1) {
            pcbarr[ppid].children.remove(Integer.valueOf(pid));
        }
        // remove process from RL or waiting list (try both)
        // - try readylist 
        readylist.remove(pid);
        // - try waitlist 
        for (int j = 0; j < RCBSIZE; j++) {
            rcbarr[j].removefromwaitlistpid(pid);
        }
        // release all resources of this process
        ListIterator<ResourceEntry> resources_Iter = pcbarr[pid].resources.listIterator(0);
        while (resources_Iter.hasNext()) {
            ResourceEntry re = resources_Iter.next();
            release(re.r, re.amount);
        }
        // remove this process from pcbarray
        pcbarr[pid] = null;
    }

    public int init() {
        pcbarr = new PCB[PCBSIZE];
        rcbarr = new RCB[RCBSIZE];
        rcbarr[0] = new RCB(1, 1);
        rcbarr[1] = new RCB(1, 1);
        rcbarr[2] = new RCB(2, 2);
        rcbarr[3] = new RCB(3, 3);
        readylist = new RL(); 
        pcbarr[0] = new PCB(-1, 0); // process 0 does not have a parent (so, -1), and priority is 0
        readylist.enqueue(0, 0); // process 0 to priority list 0
        return scheduler();
    }

    public int create(int priority) {
        if (priority < 1 || priority > 2) {
            return -1;
        }
        int runningproc = readylist.front();
        int index = pcbopenslot(); // find first opening in pcb array
        if (index == -1) {
            return -1;
        }
        if (treesize(0) == PCBSIZE) { // max processes created, cannot create more
            return -1;
        }
        pcbarr[index] = new PCB(runningproc, priority);
        pcbarr[runningproc].children.add(index); // add index of this process to parent's children list
        readylist.enqueue(priority, index); // add index of this process to ready list
        return scheduler();
    }

    public int destroy(int pid) {
        if (!processexists(pid)) {
            return -1;
        }
        if (pid == 0) {
            return -1;
        }
        // can only destroy running process and its children
        int runningprocess = readylist.front();
        LinkedList<Integer> familytree = treemembers(runningprocess);
        if (runningprocess != pid && !familytree.contains(pid)) {
            return -1;
        }
        int size = pcbarr[pid].children.size();
        if (size > 0) {
            int totaldestroyed = 0;
            // for each child process of this process destroy recusively
            for (int i = 0; i < size; i++) { 
                int child = pcbarr[pid].children.peekFirst();
                totaldestroyed += destroy(child);
            }
            remove(pid);
            return totaldestroyed + 1;
        }
        else {
            remove(pid);
            return 1;
        }
    }

    public int request(int r, int k) {
        if (r < 0 || r > 3) {
            return -1;
        }
        if (k < 1) {
            return -1;
        }
        int runningproc = readylist.front();
        if (runningproc == 0) {
            return -1;
        }
        // requested units + units held cannot exceed initial resource qty
        if (k + (pcbarr[runningproc].resourceamountheld(r)) > rcbarr[r].initialinventory()) {
            return -1;
        }
        if (rcbarr[r].state >= k) { // if resource amount is available
            rcbarr[r].state -= k; // reduce amount of resource by k
            pcbarr[runningproc].addresource(r, k); // add resource and amount to process's list of resources
        }
        else { // if resource amount not available
            pcbarr[runningproc].state = 0; // set process to blocked
            rcbarr[r].addtowaitlist(readylist.dequeue(), k); // remove running process from readylist and add pid to resource's waitlist
        }
        return scheduler();
    }
    
    public int release(int r, int k) {
        if (k < 1) {
            return -1;
        }
        // if releasing from running process, release total amount requested or none
        if (!destroying) {
            int runningproc = readylist.front();
            if (runningproc == 0) {
                return -1;
            }
            if (k <= pcbarr[runningproc].resourceamountheld(r)) {
                pcbarr[runningproc].withdrawresource(r, k);
                rcbarr[r].state += k; // add released amount back to rcb
            }
            else {
                return -1;
            }
        }
        // else if releasing units from destroyed processes
        else {
            rcbarr[r].state += k; // add amount k to resource r in corresponding rcb
        } 
        // move as many processes from waitlist to ready list as possible
        if (rcbarr[r].waitlist.isEmpty()) { // if no process is waiting for this resource
            // DO NOTHING - make if-only
        }
        else {
            while (rcbarr[r].frontwailtlistqtyrequested() <= rcbarr[r].state) {
                int qtyrequested = rcbarr[r].frontwailtlistqtyrequested();
                int headofwaitlist = rcbarr[r].removefromwaitlist();
                rcbarr[r].state -= qtyrequested;
                int headofwaitlistpriority = processpriority(headofwaitlist);
                readylist.enqueue(headofwaitlistpriority, headofwaitlist); // add to end of ready list (if priority level empty, becomes first)
                pcbarr[headofwaitlist].state = 1; // waitlisted process now ready
                pcbarr[headofwaitlist].addresource(r, qtyrequested); // add r and requested to previouly waitlisted process's list of resources
            }
        }
        return scheduler();
    }

    public int scheduler() {
        return readylist.front();
    }

    public int timeout() {
        // move running process from head of ready-list to tail
        readylist.enqueue(readylist.frontpriority(), readylist.dequeue());
        return scheduler();
    }

    public int treesize(int pid) {
        int size = pcbarr[pid].children.size();
        if (size > 0) {
            int total = 0;;
            for (int i = 0; i < size; i++) { 
                int child = pcbarr[pid].children.get(i);
                total += treesize(child);
            }
            return total + 1;
        }
        else {
            return 1;
        }
    }

    public LinkedList<Integer> treemembers(int pid) {
        int size = pcbarr[pid].children.size();
        if (size > 0) {
            LinkedList<Integer> total = new LinkedList<Integer>(); 
            for (int i = 0; i < size; i++) { 
                int child = pcbarr[pid].children.get(i);
                total.addAll(treemembers(child));
            }
            total.add(pid);
            return total;
        }
        else {
            LinkedList<Integer> nochild = new LinkedList<Integer>();
            nochild.add(pid);
            return nochild;
        }
    }

    public void processlist() {
        int runningproc = readylist.front();
        // find highest pcbarray index in use
        int highindex = 0;
        for (int i = 0; i < PCBSIZE; i++) {
            if (pcbarr[i] != null) {
                highindex = i;
            }
        }
        System.out.println("");
        for (int i = 0; i <= highindex; i++) {
            System.out.print("***** Process " + i + " *****");
            if (runningproc == i) {
                System.out.print(" R");
            }
            if (pcbarr[i] == null) {
                System.out.println("\nNull\n");
            }
            else {
                System.out.print("\npriority " + pcbarr[i].priority +
                                    "\nstate: " + pcbarr[i].state +
                                    "\nparent: " + pcbarr[i].parent +
                                    "\nchildren: " + pcbarr[i].children +
                                    "\nresources: ["); 
                for (ResourceEntry re : pcbarr[i].resources) {
                    System.out.print("(" + re.r + ", " + re.amount + ")");
                }
                System.out.print("]\n");
            }
        }
    }
    
    public void resourcelist() {
        System.out.println("");
        for (int i = 0; i < RCBSIZE; i++) {
            System.out.print("***** Resource " + i + " *****");
            System.out.print("\nstate: " + rcbarr[i].state +
                               "\nwaitlist: ["); 
            for (WaitlistEntry we : rcbarr[i].waitlist) {
                System.out.print("(" + we.pid + ", " + we.requested + ")");
            }
            System.out.print("]\n");
        }
    }

    public void readylist() {
        System.out.println("Ready List: " + readylist);
    }
}
