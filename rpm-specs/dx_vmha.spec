
Name:             vmha
Epoch:            1
Version:          1.0.0
Release:          6
Summary:          vmha

License:          ASL 2.0
URL:              http://www.openstack.org/software/openstack-storage/
Source0:          %{name}-%{version}.tar.gz

BuildArch:        noarch

%description
vmha

%prep
%setup -q

%build

%install
mkdir -p %{buildroot}/%{python2_sitelib}
mkdir -p %{buildroot}/bin
mkdir -p %{buildroot}/etc/vmha
mkdir -p %{buildroot}/usr/lib/systemd/system/
mkdir -p %{buildroot}/var/log/vmha
mkdir -p %{buildroot}/var/lib/vmha
install -m 777 bin/* %{buildroot}/bin/
cp etc/* %{buildroot}/etc/vmha/
install -m 777 service/* %{buildroot}/usr/lib/systemd/system/
cp -a vmha %{buildroot}/%{python2_sitelib}/


%pre

%post

%preun

%postun

%files
/bin/vmha-client
/bin/vmha-server
/etc/vmha/
/var/log/vmha
/var/lib/vmha
/usr/lib/systemd/system/vmha-client.service
/usr/lib/systemd/system/vmha-server.service
/%{python2_sitelib}/vmha

%changelog
