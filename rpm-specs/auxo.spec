
Name:             auxo
Epoch:            1
Version:          1.0.0
Release:          4
Summary:          auxo

License:          ASL 2.0
URL:              http://www.openstack.org/software/openstack-storage/
Source0:          %{name}-%{version}.tar.gz

BuildArch:        noarch

%description
vmha

%prep
%setup -q
%autosetup -n auxo-%{version} -S git

%build
%{__python2} setup.py build

%install
%{__python2} setup.py install -O1 --skip-build --root %{buildroot}
mkdir -p %{buildroot}/var/log/auxo/
compile_config="/root/auxo.conf"
python /root/deletepy.py %{buildroot} ${compile_config}


%pre

%post

%preun

%postun

%files
/usr/bin/auxo-api
/usr/bin/auxo-config-compute
/usr/bin/auxo-config-controller
/usr/bin/auxo-manage
/usr/bin/auxo-network
/usr/bin/auxo-node
/usr/bin/auxo-server
/usr/bin/auxo-storage
/usr/lib/systemd/system/auxo-*.service
/etc/auxo
/var/log/auxo/
%{python2_sitelib}/auxo
%{python2_sitelib}/auxo-*.egg-info

%changelog
