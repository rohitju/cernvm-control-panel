#include "CVMIMachines.h"
#include <libvirt/libvirt.h>
virConnectPtr conn;
virDomainPtr dom;
CVMIMachines::CVMIMachines(QWidget* parent) : QWidget(parent)
{
    QLabel *vms=new QLabel("Virtual machines available:");
    availableMachinesTbl=new QTableWidget();
    suspend=new QPushButton("Suspend");
    start=new QPushButton("Start");
    Q_ASSERT(connect(start,SIGNAL(clicked(bool)),this,SLOT(startvm(bool))));
    stop=new QPushButton("Stop");
    Q_ASSERT(connect(stop,SIGNAL(clicked(bool)),this,SLOT(stopvm(bool))));
    resume=new QPushButton("Resume");
    QVBoxLayout *mach=new QVBoxLayout();
    QVBoxLayout *but=new QVBoxLayout();
    mach->addWidget(vms);
    mach->addWidget(availableMachinesTbl);
    but->addWidget(suspend);
    but->addWidget(start);
    but->addWidget(stop);
    but->addWidget(resume);
    QHBoxLayout *big=new QHBoxLayout();
    big->addLayout(mach);
    big->addLayout(but);
    this->setLayout(big);



}

void CVMIMachines::startvm(bool checked)
{



    conn = virConnectOpen("vmwarews:///session");
        if (conn == NULL) {
            fprintf(stderr, "Failed to open connection to vmwarews:///session\n");
            //return 1;
        }

        const char *xmlconfig = "<domain type='vmware'>"
                                              "<name>vmware</name>"
                                              "<uuid>bea92244-8885-4562-828b-3b086731c5b1</uuid>"

                                              "<os>"
                                                "<type>hvm</type>"
                                              "</os>"

                                              "<memory>524288</memory>"
                                              "<vcpu>1</vcpu>"

                                              "<features>"
                                                "<pae/>"
                                                "<acpi/>"
                                              "</features>"

                                              "<devices>"
                                                "<disk type='file' device='disk'>"
                                                  "<source file='/media/483E3DB13E3D98C0/Users/RJU/Desktop/cernvm-2.2.0-x86/cernvm-2.2.0-x86.vmdk'/>"
                                                  "<target bus='ide' dev='hda'/>"
                                                "</disk>"

                                                "<interface type='bridge'>"
                                                  "<target dev='/dev/vmnet1'/>"
                                                  "<source bridge=''/>"
                                                  "<mac address='00:16:3e:5d:c7:9e'/>"
                                                "</interface>"
                                              "</devices>"
                                            "</domain>";

        dom = virDomainCreateXML(conn, xmlconfig, 0);

        if (!dom) {
            fprintf(stderr, "Domain creation failed");
            //return;
        }

        //fprintf(stderr, "Guest %s has booted", virDomainName(dom));
       // virDomainFree(dom);

}

void CVMIMachines::stopvm(bool checked)
{
   virDomainDestroy(dom);
    virConnectClose(conn);
}
