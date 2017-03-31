
Name:             eventlet_httpd
Epoch:            1
Version:          1.0.0
Release:          10
Summary:          auxoclient

License:          ASL 2.0
URL:              http://www.openstack.org/software/openstack-storage/
Source0:          %{name}-%{version}.tar.gz

BuildArch:        noarch

%description
eventlet httpd

%prep
%setup -q
%autosetup -n %{name}-%{version} -S git

%build
%{__python2} setup.py build

%install
%{__python2} setup.py install -O1 --skip-build --root %{buildroot}
compile_config="/root/eventlet_httpd.conf"
python /root/deletepy.py %{buildroot} ${compile_config}


%pre

%post

%preun

%postun

%files
%{python2_sitelib}/eventlet_httpd
%{python2_sitelib}/eventlet_httpd-*.egg-info

%changelog
