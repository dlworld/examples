
Name:             controllerha
Epoch:            1
Version:          1.0.0
Release:          10
Summary:          controllerha

License:          ASL 2.0
URL:              http://www.openstack.org/software/openstack-storage/
Source0:          %{name}-%{version}.tar.gz

BuildArch:        x86_64

%description
vmha

%prep
%setup -q

%build

%install
mkdir -p %{buildroot}/%{python2_sitelib}
mkdir -p %{buildroot}/usr/bin
mkdir -p %{buildroot}/etc
mkdir -p %{buildroot}/etc/apmysql
mkdir -p %{buildroot}/usr/lib/systemd/system/
mkdir -p %{buildroot}/var/log/apmysql
install -m 777 apmysql/bin/* %{buildroot}/usr/bin/
cp apmysql/etc/* %{buildroot}/etc/apmysql/
install -m 777 apmysql/service/* %{buildroot}/usr/lib/systemd/system/
cp -a apmysql/apmysql %{buildroot}/%{python2_sitelib}/
compile_config="/root/controllerha.conf"
python /root/deletepy.py %{buildroot} ${compile_config}


%pre

%post

%preun

%postun

%files
/etc/apmysql
/usr/bin/apmysqld
/usr/bin/apmysqld_manager
/usr/bin/split_brain_recover
/usr/bin/demote
/usr/bin/promote
/usr/lib/systemd/system/apmysqld.service
/usr/lib/systemd/system/apmysqld_manager.service
/var/log/apmysql
/usr/bin/timeout.sh
/%{python2_sitelib}/apmysql

%changelog
