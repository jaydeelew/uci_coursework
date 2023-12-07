import java.util.StringTokenizer;

public class VMManager {
    private static final int PMSZ = 524288;
    private static final int FRAMESZ = 512;
    private static final int BLOCKSZ = 512;

    private int[] PM = new int[PMSZ];
    private int[][] Disk = new int[1024][512]; // shouldn't if be 262144 rows? 2^18 rows x 2^9 columns = 2^27 (27 bits in VA)

    public void initST(StringTokenizer st) {
        while (st.hasMoreTokens()) {
            int segment = Integer.parseInt(st.nextToken());
            int segsize = Integer.parseInt(st.nextToken());
            int frame = Integer.parseInt(st.nextToken());

            PM[2*segment] = segsize;
            PM[2*segment + 1] = frame; // element contains start of pagetable (frame-number or disk-block-number) of segment
                                       // disk-block will be a negative integer
        }
    } 

    public void initPTs(StringTokenizer st) {
        while (st.hasMoreTokens()) {
            int segment = Integer.parseInt(st.nextToken());
            int page = Integer.parseInt(st.nextToken());
            int frame = Integer.parseInt(st.nextToken());

            if (PM[2*segment + 1] < 0) { // if page-table resides on Disk
                int blocknum = Math.abs(PM[2*segment + 1]);
                Disk[blocknum][page] = frame; // the page of segment's pagetable is located in this frame/block 
            }
            else { // else page-table in Physical Memory 
                /*
                pm[2*segment + 1]                            -element contains frame-number of segment
                pm[2*segment + 1]*512                        -frame-number * size of a frame = index of start of frame holding pagetable
                pm[2*segment + 1]*512 + page                 -page is the offset into the pagetable to start of page
                pm[(pm[2*segment + 1])*512 + page] = frame   -the page of segment's pagetable is located in this frame/block
                */ 
                PM[(PM[2*segment + 1])*512 + page] = frame;
            }
        }
    }

    // read a diskBlock from Disk, copy the block to array beginning at arrayPosition
    private void read_block(int diskBlock, int[] array, int arrayPosition) {
        for (int i = arrayPosition, j = 0; i < arrayPosition + BLOCKSZ; i++, j++) {
            array[i] = Disk[diskBlock][j];
        }
    }

    private int firstfreeframe() {
        int[] allocatedFrames = new int[1024];
        allocatedFrames[0] = 1; // for ST
        allocatedFrames[1] = 1; // for ST
        // find all PTs and pages in memory
        for (int i = 0; i < FRAMESZ; i++) { // for the 512 segments in ST
            int ptFrameNum = PM[i*2 + 1]; // frame/block num of each PT
            int ptMemoryIdx = ptFrameNum * FRAMESZ; // pagetable location in PM[]
            if (ptFrameNum > 0) { // if pagetable is in memory, allocate frame
                allocatedFrames[ptFrameNum] = 1; 
                for (int j = ptMemoryIdx; j < ptMemoryIdx + FRAMESZ; j++) { // for each page in PT
                    int pageFrameNum = PM[j];
                    if (pageFrameNum > 0) { // if page is in memory, allocate frame
                        allocatedFrames[pageFrameNum] = 1; 
                    }
                }
            }
            else if (ptFrameNum < 0) { // if pagetable is not im memory, do not allocate frame
                int ptBlockNum = Math.abs(ptFrameNum);
                int[] blockArray = new int[512];
                read_block(ptBlockNum, blockArray, 0);
                for (int k = 0; k < BLOCKSZ; k++) {
                    int pageFrameNum = blockArray[k];
                    if (pageFrameNum > 0) { // if page is in memory, allocate frame
                        allocatedFrames[pageFrameNum] = 1; 
                    }
                }
            }
        }
        // return first free frame in PM[]
        for (int i = 0; i < 1024; i++) {
            if (allocatedFrames[i] == 0) {
                return i;
            }
        }
        return -1;
    }

    public int translateaddress(String address) {
        int va;
        int pa;
        try {
            va = Integer.parseInt(address);
        } catch (NumberFormatException e) {
            return -1;
        }
        int s = va >> 18; // segment number
        if (s < 0 || s > 511) { // if segment number is not within the ST
            return -1;
        }
        int p = va >> 9; // page of pagetable (step 1)
        p = p & 0x1FF; // page of pagetable (step 2)
        int w = va & 0x1FF; // word offset in page
        int pw = va & 0x3FFFF; // address offset in segment
        if (pw >= PM[2*s]) { // if va is beyond segment end
            return -1;
        }
        int ptFrameNum = PM[2*s + 1]; // pagetable memory-frame number
        
        if (ptFrameNum > 0) { // if pagetable is in memory
            int ptMemoryIdx = ptFrameNum * FRAMESZ; // pagetable location in PM[]
            if (PM[ptMemoryIdx + p] < 0) { // if page is on Disk[][]
                PM[ptMemoryIdx + p] = firstfreeframe(); // page now in memory               
            }
        }
        else if (ptFrameNum < 0) { // if pagetable is on Disk[][]
            int ptBlockNum = Math.abs(ptFrameNum); // pagetable disk-block number
            int freeframe = firstfreeframe();
            PM[2*s + 1] = freeframe; // update pagetable frame in ST
            int ptMemoryIdx = freeframe * FRAMESZ; // new location of pagetable 

            read_block(ptBlockNum, PM, ptMemoryIdx);

            if (PM[ptMemoryIdx + p] < 0) { // if page is on Disk[][]
                PM[ptMemoryIdx + p] = firstfreeframe(); // page now in memory               
            }
        }
        pa = PM[PM[2*s + 1]*512 + p]*512 + w;
        return pa;
    }
}