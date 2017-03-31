
Name:             oestorage
Epoch:            1
Version:          1.0.0
Release:          4
Summary:          oestorage

License:          ASL 2.0
URL:              http://www.openstack.org/software/openstack-storage/
Source0:          %{name}-%{version}.tar.gz

BuildArch:        noarch

%description
vmha

%prep
%setup -q

%build
%{__python2} setup.py build

%install
%{__python2} setup.py install -O1 --skip-build --root %{buildroot}
compile_config="/root/oestorage.conf"
python /root/deletepy.py %{buildroot} ${compile_config}


%pre

%post

%preun

%postun

%files
%{python2_sitelib}/oestorage
%{python2_sitelib}/python_oestorage-*.egg-info

%changelog
