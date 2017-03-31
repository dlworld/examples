
Name:             ceilometer
Epoch:            1
Version:          5.0.0
Release:          4
Summary:          ceilometer

License:          ASL 2.0
URL:              http://www.openstack.org/software/openstack-storage/
Source0:          %{name}-%{version}.tar.gz

BuildArch:        noarch

%description
ceilometer

%prep
%setup -q
%autosetup -n ceilometer-%{version} -S git

%build
%{__python2} setup.py build

%install
%{__python2} setup.py install -O1 --skip-build --root %{buildroot}
mkdir -p %{buildroot}/var/log/ceilometer/
compile_config="/root/ceilometer.conf"
python /root/deletepy.py %{buildroot} ${compile_config}


%pre

%post

%preun

%postun

%files
/usr/bin/ceilometer-api
/usr/bin/ceilometer-polling
/usr/bin/ceilometer-agent-notification
/usr/bin/ceilometer-send-sample
/usr/bin/ceilometer-dbsync
/usr/bin/ceilometer-expirer
/usr/bin/ceilometer-rootwrap
/usr/bin/ceilometer-collector
/usr/bin/ceilometer-alarm-evaluator
/usr/bin/ceilometer-alarm-notifier
#/etc/ceilometer
/var/log/ceilometer/
%{python2_sitelib}/ceilometer
%{python2_sitelib}/ceilometer-*.egg-info

%changelog
