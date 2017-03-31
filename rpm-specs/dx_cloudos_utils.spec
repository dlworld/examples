
Name:             cloudos_utils
Epoch:            1
Version:          1.0.0
Release:          5
Summary:          cloudos_utils

License:          ASL 2.0
URL:              http://www.openstack.org/software/openstack-storage/
Source0:          %{name}-%{version}.tar.gz

BuildArch:        x86_64

%description
cloudos_utils

%prep
%setup -q

%build

%install
mkdir -p %{buildroot}/%{python2_sitelib}
mkdir -p %{buildroot}/lib64
mkdir -p %{buildroot}/usr/lib64/python2.7
mkdir -p %{buildroot}/usr/lib64/python2.7/lib-dynload
install -m 777 lib/*.py %{buildroot}/usr/lib64/python2.7/
install -m 644 lib/_monotonic_time.so %{buildroot}/usr/lib64/python2.7/lib-dynload/
install -m 644 lib/hardinfo.so %{buildroot}/lib64/
cp -a cloudos_utils %{buildroot}/%{python2_sitelib}/


%pre

%post

%preun

%postun

%files
/%{python2_sitelib}/cloudos_utils
/usr/lib64/python2.7/lib-dynload/_monotonic_time.so
/lib64/hardinfo.so
/usr/lib64/python2.7/monotonic_Queue.py
/usr/lib64/python2.7/monotonic_threading.py
/usr/lib64/python2.7/monotonic_time.py
/usr/lib64/python2.7/monotonic_Queue.pyo
/usr/lib64/python2.7/monotonic_threading.pyo
/usr/lib64/python2.7/monotonic_time.pyo
/usr/lib64/python2.7/monotonic_Queue.pyc
/usr/lib64/python2.7/monotonic_threading.pyc
/usr/lib64/python2.7/monotonic_time.pyc


%changelog
