#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <Hypervisor/Hypervisor.h>

using namespace std;

int main()
{

    hv_return_t vm_status = hv_vm_create(nullptr);
    if (vm_status != HV_SUCCESS)
    {
        cerr << "[-] Error: Gagal inisialisasi Hypervisor." << endl;
        cerr << "[!] Pastikan 'sudo' digunakan dan Entitlements terpasang." << endl;
        return 1;
    }


    const size_t mem_size = 0x4000;
    void *host_mem = nullptr;

    if (posix_memalign(&host_mem, 0x4000, mem_size) != 0) {
        cerr << "[-] Gagal alokasi memori aligned." << endl;
        return 1;
    }
    memset(host_mem, 0, mem_size);

    // 3. Mapping Memori ke Guest (GPA)
    // Kita petakan RAM host ke alamat 0x4000 di dalam VM
    uint64_t guest_addr = 0x4000;
    hv_return_t map_status = hv_vm_map(host_mem, guest_addr, mem_size, 
                                      HV_MEMORY_READ | HV_MEMORY_WRITE | HV_MEMORY_EXEC);

    if (map_status != HV_SUCCESS)
    {
        cerr << "[-] Gagal memetakan memori ke VM. Error code: " << map_status << endl;
        free(host_mem);
        return 1;
    }

    hv_vcpu_t vcpu;
    hv_vcpu_exit_t *exit_info;
    hv_return_t vcpu_status = hv_vcpu_create(&vcpu, &exit_info, nullptr);

    if (vcpu_status == HV_SUCCESS)
    {
        cout << "----------------------------------------" << endl;
        cout << "      CORE-BREAKER KERNEL LAB ACTIVE     " << endl;
        cout << "----------------------------------------" << endl;
        cout << "[+] Hardware Virtualization: ENABLED" << endl;
        cout << "[+] vCPU State: INITIALIZED" << endl;
        cout << "[+] Memory Mapping: SUCCESS (GPA: 0x4000)" << endl;


        uint32_t code[] = {
            0xd28266e0, 
            0xd2800021, 
            0x8b010002  
        };


        memcpy(host_mem, code, sizeof(code));


        hv_vcpu_set_reg(vcpu, HV_REG_PC, guest_addr);
        

        hv_vcpu_set_reg(vcpu, HV_REG_X2, 0);


        cout << "[*] Executing payload inside sandbox..." << endl;
        hv_return_t run_status = hv_vcpu_run(vcpu);

        if (run_status != HV_SUCCESS) {
            cerr << "[-] Gagal menjalankan vCPU." << endl;
        } else {

            uint64_t x2_val;
            hv_vcpu_get_reg(vcpu, HV_REG_X2, &x2_val);

            cout << "----------------------------------------" << endl;
            cout << "[!] EXECUTION RESULT [!]" << endl;
            cout << "[+] Register X2 (0x1337 + 1) = 0x" << hex << x2_val << endl;
            cout << "----------------------------------------" << endl;
        }
        

        uint64_t pfr0_val;
        hv_vcpu_get_sys_reg(vcpu, HV_SYS_REG_ID_AA64PFR0_EL1, &pfr0_val);
        cout << "[+] Processor Feature Reg (EL1): 0x" << hex << pfr0_val << endl;
    }


    hv_vm_destroy();
    free(host_mem);
    cout << "[*] System session closed." << endl;

    return 0;
}