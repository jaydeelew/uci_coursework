import java.util.LinkedList;

class WaitlistEntry {
    public int pid;
    public int requested;

    public WaitlistEntry(int pid, int requested) {
        this.pid = pid;
        this.requested = requested;
    }
}

public class RCB {
    public int inventory; // the max number of resources ever available 
    public int state; // the current level of resources available
    public LinkedList<WaitlistEntry> waitlist; // indices and units requested of processes blocked on this resource
    
    public RCB(int inventory, int state) {
        this.inventory = inventory;
        this.state = state;
        waitlist = new LinkedList<WaitlistEntry>();
    }

    public void addtowaitlist(int pid, int requested) {
        WaitlistEntry we = new WaitlistEntry(pid, requested);
        waitlist.add(we);
    }

    public int removefromwaitlist() {
        WaitlistEntry we = waitlist.peekFirst();
        int pid = we.pid;
        waitlist.removeFirst();
        return pid;
    }

    public void removefromwaitlistpid(int pid) {
        for (int i = 0; i < waitlist.size(); i++) {
            if (waitlist.get(i).pid == pid) {
                waitlist.remove(i);
                break;
            }
        }
    }

    public int frontwailtlistqtyrequested() {
        if (waitlist.peekFirst() != null) {
            return waitlist.peekFirst().requested;
        }
        else {
            return 99; // large number to prevent entering while loop in release()
        }
    }

    public int initialinventory() {
        return inventory;
    }

    public String toString(){ // overidden toString()
        return "\ninventory: " + this.inventory + 
               "\nstate: " + this.state +
               "\nchildren: " + this.waitlist.toString();
    } 
}
