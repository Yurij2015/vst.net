#pragma once

namespace Jacobi {
namespace Vst {
namespace Interop {
namespace Host {

	/// <summary>
	/// The VstPluginContext class represents a VST Plugin for the host.
	/// </summary>
	/// <remarks>
	/// The VstPluginContext can only be created using the static 
	/// <see cref="Jacobi::Vst::Interop::Host::VstPluginContext::Create"/> 
	/// factory method. The method accepts the path to the plugin and an
	/// implementation of the <see cref="Jacobi::Vst::Core::Host::IVstHostCommandStub"/>
	/// interface.
	/// Note that <c>Open</c> is not called on the Plugin by the context but it does call 
	/// Close on the plugin when the context is disposed.
	/// </remarks>
	public ref class VstPluginContext abstract : public Jacobi::Vst::Core::Host::IVstPluginContext, public System::IDisposable
	{
	public:
		// IDisposable interface implementation
		~VstPluginContext();

		/// <summary>
		/// Creates a context using the specified <paramref name="pluginPath"/> and <paramref name="hostCmdStub"/>.
		/// </summary>
		/// <param name="pluginPath">The full path to a plugin .dll. Must not be null or empty.</param>
		/// <param name="hostCmdStub">A reference to a host supplied implementation of the host command stub. Must not be null.</param>
		/// <remarks>Note that when the <paramref name="hostCmdStub"/> implements <see cref="System::IDisposable"/> 
		/// it is also disposed when this instance of the plugin context is disposed.</remarks>
		/// <exception cref="System::IO::FileNotFoundException">Thrown when the file specified in 
		/// <paramref name="pluginPath"/> does not exist.</exception>
		/// <exception cref="System::ArgumentException">Thrown when the library cannot be opened.</exception>
		/// <exception cref="System::EntryPointNotFoundException">Thrown when the library does not expose a 
		/// <c>VSTPluginMain</c> exported function.</exception>
		/// <exception cref="System::OperationCanceledException">Thrown when the library does not return 
		/// an ::EAffect pointer from its exported main function.</exception>
		/// <exception cref="System::NotSupportedException">Thrown when the library does not specify the correct magic number
		/// in the ::EAffect structure or when it does not support VST version 2.4.</exception>
		static VstPluginContext^ Create(System::String^ pluginPath, Jacobi::Vst::Core::Host::IVstHostCommandStub^ hostCmdStub);
		
		// IVstPluginContext interface implementation
		/// <summary>
		/// Sets a new <paramref name="value"/> for the <paramref name="keyName"/> property.
		/// </summary>
		/// <typeparam name="T">Inferred, no need to specify explicitly.</typeparam>
		/// <param name="keyName">The name of the key used to identify the property.</param>
		/// <param name="value">The value of the property.</param>
		/// <remarks>When an existing value is overwritten the <see cref="PropertyChanged"/> event fires.
		/// When the <paramref name="value"/> implements <see cref="System::IDisposable"/> it is called
		/// when this instance of the plugin context is disposed.
		/// </remarks>
		generic<typename T> 
		virtual void Set(System::String^ keyName, T value);
		/// <summary>
		/// Retrieves the value for the <paramref name="keyName"/> property.
		/// </summary>
		/// <typeparam name="T">The data type of the property.</typeparam>
		/// <param name="keyName">The name of the key used to identify the property.</param>
		/// <returns>Returns the value of the property identified by <paramref name="keyName"/>
		/// or default(T) when it was not found.</returns>
		generic<typename T> 
		virtual T Find(System::String^ keyName);
		/// <summary>
		/// Removes the <paramref name="keyName"/> property from the plugin context.
		/// </summary>
		/// <param name="keyName">The name of the key used to identify the property.</param>
		/// <remarks><see cref="System::IDisposable::Dispose"/> is <b>NOT</b> called on the property.</remarks>
		virtual void Remove(System::String^ keyName)
		{ _props->Remove(keyName); }
		/// <summary>
		/// Deletes the <paramref name="keyName"/> property and removes it from the plugin context.
		/// </summary>
		/// <param name="keyName">The name of the key used to identify the property.</param>
		/// <remarks><see cref="System::IDisposable::Dispose"/> <i>is</i> called on the property (when implemented).</remarks>
		virtual void Delete(System::String^ keyName);
		
		/// <summary>
		/// Gets the reference to the host command stub passed into the ctor.
		/// </summary>
		virtual property Jacobi::Vst::Core::Host::IVstHostCommandStub^ HostCommandStub 
		{ Jacobi::Vst::Core::Host::IVstHostCommandStub^ get() { return _hostCmdStub; } }

		/// <summary>
		/// Gets the reference to the plugin command stub.
		/// </summary>
		virtual property Jacobi::Vst::Core::Host::IVstPluginCommandStub^ PluginCommandStub 
		{
			Jacobi::Vst::Core::Host::IVstPluginCommandStub^ get() { return _pluginCmdStub; }
			protected: void set(Jacobi::Vst::Core::Host::IVstPluginCommandStub^ value) { _pluginCmdStub = value; }
		}

		/// <summary>
		/// Gets the reference to the plugin info.
		/// </summary>
		virtual property Jacobi::Vst::Core::Plugin::VstPluginInfo^ PluginInfo 
		{
			Jacobi::Vst::Core::Plugin::VstPluginInfo^ get() { return _pluginInfo; }
			void set(Jacobi::Vst::Core::Plugin::VstPluginInfo^ value) { _pluginInfo = value; }
		}

		/// <summary>
		/// Copies the unmanaged property values to the <see cref="PluginInfo"/> properties.
		/// </summary>
		/// <param name="raiseEvents">When true the <see cref="PropertyChanged"/> event 
		/// is raised for each property that has changed.</param>
		virtual void AcceptPluginInfoData(System::Boolean raiseEvents) abstract;

		// INotifyPropertyChanged interface implementation
		/// <summary>
		/// Raised when a property is changed on the plugin context.
		/// </summary>
		virtual event System::ComponentModel::PropertyChangedEventHandler^ PropertyChanged;

	protected:
		VstPluginContext(Jacobi::Vst::Core::Host::IVstHostCommandStub^ hostCmdStub);

		void RaisePropertyChanged(System::String^ propName)
		{ PropertyChanged(this, gcnew System::ComponentModel::PropertyChangedEventArgs(propName));}

	private:
		Jacobi::Vst::Core::Host::IVstHostCommandStub^ _hostCmdStub;
		Jacobi::Vst::Core::Host::IVstPluginCommandStub^ _pluginCmdStub;
		Jacobi::Vst::Core::Plugin::VstPluginInfo^ _pluginInfo;

		// contains all user properties
		System::Collections::Generic::Dictionary<System::String^, System::Object^>^ _props;

		static VstPluginContext^ InitializeManaged(System::String^ pluginPath, Jacobi::Vst::Core::Host::IVstHostCommandStub^ hostCmdStub);
		static VstPluginContext^ InitializeUnmanaged(System::String^ pluginPath, Jacobi::Vst::Core::Host::IVstHostCommandStub^ hostCmdStub);
	};

}}}} // namespace Jacobi::Vst::Interop::Host

