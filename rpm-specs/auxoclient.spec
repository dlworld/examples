
Name:             python-auxoclient
Epoch:            1
Version:          1.0.0
Release:          4
Summary:          python-auxoclient

License:          ASL 2.0
URL:              http://www.openstack.org/software/openstack-storage/
Source0:          %{name}-%{version}.tar.gz

BuildArch:        noarch

%description
auxo client

%prep
%setup -q
%autosetup -n %{name}-%{version} -S git

%build
%{__python2} setup.py build

%install
%{__python2} setup.py install -O1 --skip-build --root %{buildroot}
compile_config="/root/auxoclient.conf"
python /root/deletepy.py %{buildroot} ${compile_config}



%pre

%post

%preun

%postun

%files
%{python2_sitelib}/auxoclient
%{python2_sitelib}/auxoclient-*.egg-info

%changelog
