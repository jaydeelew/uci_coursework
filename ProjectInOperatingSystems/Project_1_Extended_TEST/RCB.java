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
    // the max number of resources ever available 
    public int inventory;
    // the current level of resources available
    public int state;
    // indices and units requested of processes blocked on this resource
    public LinkedList<WaitlistEntry> waitlist;
    
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
            // large number to prevent entering while loop in release()
            return 99;
        }
    }

    public int initialinventory() {
        return inventory;
    }

    // overidden toString()
    public String toString(){
        return "\ninventory: " + this.inventory + 
               "\nstate: " + this.state +
               "\nchildren: " + this.waitlist.toString();
    } 
}
