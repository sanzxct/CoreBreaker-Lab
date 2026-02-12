#include <iostream>
#include <Hypervisor/Hypervisor.h>

using namespace std;

int main(){

    hv_return_t vm_status = hv_vm_create(nullptr);

    if(vm_status != HV_SUCCESS) {
        cerr <<"[-] Error: Gagal inisialisasi Hypervisor." << endl;
        cerr << "[!] Pastikan 'sudo' digunakan dan Entitlements terpasang." << endl;
        return 1;
    }

    hv_vcpu_t vcpu;
    hv_vcpu_exit_t *exit_info;

    hv_return_t vcpu_status = hv_vcpu_create(&vcpu, &exit_info, nullptr);

    if(vcpu_status == HV_SUCCESS) {
        cout << "----------------------------------------" << endl;
        cout << "      CORE-BREAKER KERNEL LAB ACTIVE     " << endl;
        cout << "----------------------------------------" << endl;
        cout << "[+] Hardware Virtualization: ENABLED" << endl;
        cout << "[+] vCPU State: INITIALIZED" << endl;
        
        // Membaca ID fitur Prosesor (PFR0) untuk memverifikasi akses level rendah
        uint64_t pfr0_val;
        hv_vcpu_get_sys_reg(vcpu, HV_SYS_REG_ID_AA64PFR0_EL1, &pfr0_val);
        cout << "[+] Processor Feature Reg (EL1): 0x" << hex << pfr0_val << endl;
        cout << "[+] Ready for Day 2: Memory Mapping & Assembly." << endl;
    
    }

    hv_vm_destroy();
    cout << "----------------------------------------" << endl;
    cout << "[*] System session closed." << endl;

    return 0;
}